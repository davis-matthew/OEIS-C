/*
 * A075156  Binomial transform of pentanacci numbers A074048: a(n) =
 *          Sum_{k=0..n} binomial(n,k)*A074048(k).
 * https://oeis.org/A075156
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6*a(n‑1) – 13*a(n‑2) + 14*a(n‑3) – 7*a(n‑4) + 2*a(n‑5),
 *            a(0) = 5, a(1) = 6, a(2) = 10, a(3) = 24, a(4) = 70
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses a simple linear‑recurrence cache.  It works for any
   integer type seq_t (unsigned 8/16/32/64/128 bits or an arbitrary‑precision
   integer class), and for any index type idx_t (normally uint64_t).  The
   implementation follows the requirements: it only uses integer
   arithmetic and standard library facilities; it memoises results and
   supports out‑of‑order calls.  It compiles under C11 and C++17. */

#include <stdlib.h>          /* malloc, realloc, free */
#include <stddef.h>          /* size_t */

/* Static cache for memoisation */
static seq_t *cache   = (seq_t *)0;   /* pointer to array of seq_t */
static size_t  cap    = 0;            /* allocated capacity (>= last+1) */
static idx_t   last   = (idx_t)-1;    /* index of the last computed term */

/* Ensure that cache has capacity for indices up to n (inclusive) */
static void
ensure_cache(idx_t n)
{
    /* First allocation: if cache is empty */
    if (cache == (seq_t *)0) {
        /* Allocate a small power‑of‑two size for convenience */
        size_t init_cap = (n < 16) ? 16 : ((size_t)n + 1);
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (cache == (seq_t *)0) {
            /* Allocation failed – abort */
            abort();
        }
        cap  = init_cap;
        /* Initialise the first five terms */
        cache[0] = (seq_t)5;
        cache[1] = (seq_t)6;
        cache[2] = (seq_t)10;
        cache[3] = (seq_t)24;
        cache[4] = (seq_t)70;
        last   = 4;
        /* If the requested index is already within the initial bounds
           we can return immediately. */
        if ((size_t)n <= last) return;
    }

    /* Grow the array if necessary */
    if ((size_t)n < cap) return;  /* already enough space */

    size_t new_cap = cap;
    while (new_cap <= (size_t)n) {
        if (new_cap > SIZE_MAX / 2) {
            /* Capacity would overflow – abort */
            abort();
        }
        new_cap <<= 1;
    }

    seq_t *tmp = (seq_t *)realloc((void *)cache, new_cap * sizeof(seq_t));
    if (tmp == (seq_t *)0) {
        /* Reallocation failed – abort */
        abort();
    }
    cache = tmp;
    cap   = new_cap;
}

/* Compute a(n) */
seq_t
a(idx_t n)
{
    /* Return the initial segment without cache overhead */
    if (n <= (idx_t)4) {
        static const seq_t init[5] = {
            (seq_t)5, (seq_t)6, (seq_t)10,
            (seq_t)24, (seq_t)70
        };
        return init[(size_t)n];
    }

    /* Ensure the cache is large enough and that earlier terms are known. */
    ensure_cache(n);

    /* If the requested term has already been computed, simply return it. */
    if (n <= last) {
        return cache[(size_t)n];
    }

    /* Compute terms from (last+1) up to n inclusively. */
    for (idx_t i = last + (idx_t)1; i <= n; ++i) {
        /* Compute the recurrence in a way that prevents temporary
           underflow when using unsigned arithmetic. */
        seq_t pos = (seq_t)6  * cache[(size_t)(i-1)] +
                    (seq_t)14 * cache[(size_t)(i-3)] +
                    (seq_t)2  * cache[(size_t)(i-5)];
        seq_t neg = (seq_t)13 * cache[(size_t)(i-2)] +
                    (seq_t)7  * cache[(size_t)(i-4)];
        cache[(size_t)i] = pos - neg;   /* guaranteed non‑negative */
    }

    last = n;
    return cache[(size_t)n];
}
