/*
 * A235064  Number of (n+1) X (1+1) 0..6 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 3,
 *          with no adjacent elements equal (constant-stress tilted 1 X 1
 *          tilings).
 * https://oeis.org/A235064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 24, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for OEIS A235064:
   a(n) = 35*a(n-2) – 235*a(n-4) + 543*a(n-6) – 458*a(n-8) + 96*a(n-10)
   for n ≥ 11, with initial values
   a(1)=168, a(2)=848, a(3)=4352, a(4)=22576, a(5)=117232,
   a(6)=609412, a(7)=3167392, a(8)=16467664, a(9)=85596352,
   a(10)=445030452.
   The function memoises results in a static dynamic array so that
   any order of calls is efficient. */

#include <stddef.h>
#include <stdlib.h>

/* Lazy cache – grows as needed.  The cache holds a(n) for n=0 … max_computed.
   a(0) is unused but occupies index 0 to keep indices simple. */
static seq_t *cache = NULL;
static size_t cache_size = 0;       /* allocated size (elements)   */
static idx_t  max_computed = 0;     /* highest n with a(n) in cache */

/* Initial values for n=1 … 10.  Index 0 is unused. */
static const seq_t init_vals[11] = {
    (seq_t)0,                           /* dummy for a(0) – never used   */
    (seq_t)168,      /* a(1) */
    (seq_t)848,      /* a(2) */
    (seq_t)4352,     /* a(3) */
    (seq_t)22576,    /* a(4) */
    (seq_t)117232,   /* a(5) */
    (seq_t)609412,   /* a(6) */
    (seq_t)3167392,  /* a(7) */
    (seq_t)16467664, /* a(8) */
    (seq_t)85596352, /* a(9) */
    (seq_t)445030452/* a(10) */
};

static void ensure_cache_cap(idx_t need)
{
    /* Need space for indices 0 … need  (size = need+1)  */
    size_t required = (size_t)(need) + 1;
    if (required <= cache_size) return;

    /* Allocate new buffer (grow by 2× to keep amortised O(1)). */
    size_t new_size = cache_size ? cache_size * 2 : 64;
    if (new_size < required) new_size = required;

    seq_t *new_cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) abort(); /* out‑of‑memory */

    cache = new_cache;
    cache_size = new_size;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* behaviour below offset is irrelevant */

    /* Initialise cache for the first call */
    if (!cache) {
        cache_size = 64;
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        if (!cache) abort();      /* out‑of‑memory */
    }

    /* Store the initial ten values once */
    if (max_computed < 10) {
        for (idx_t i = 1; i <= 10; ++i) {
            cache[(size_t)i] = init_vals[(size_t)i];
        }
        max_computed = 10;
    }

    /* If the requested n is already known, return it */
    if (n <= max_computed) return cache[(size_t)n];

    /* Ensure we have room for all needed indices */
    ensure_cache_cap(n);

    /* Compute missing terms using the recurrence */
    /* We can start at the first n > max_computed we are asked for,
       but only if n ≥ 11.  For n < 11 the answer was already cached. */
    idx_t start = (max_computed + 1 > 11 ? max_computed + 1 : 11);

    for (idx_t i = start; i <= n; ++i) {
        seq_t a2  = cache[(size_t)(i-2)];
        seq_t a4  = cache[(size_t)(i-4)];
        seq_t a6  = cache[(size_t)(i-6)];
        seq_t a8  = cache[(size_t)(i-8)];
        seq_t a10 = cache[(size_t)(i-10)];

        /* All arithmetic is done in seq_t; intermediate results fit in seq_t
           because the recurrence uses only additions and subtractions, not
           divisions. */
        seq_t pos = (seq_t)35 * a2 + (seq_t)543 * a6 + (seq_t)96 * a10;
        seq_t neg = (seq_t)235 * a4 + (seq_t)458 * a8;
        cache[(size_t)i] = pos - neg;
    }

    max_computed = n;
    return cache[(size_t)n];
}
