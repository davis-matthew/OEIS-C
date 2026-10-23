/*
 * A186467  Number of (n+1) X 5 0..2 arrays with every 2 X 2 subblock
 *          commuting with each of its horizontal and vertical 2 X 2 subblock
 *          neighbors.
 * https://oeis.org/A186467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 9, u32 n <= 27, u64 n <= 64, u128 n <= 138,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A186467.
   Recurrence for n > 13:
      a(n) = 2*a(n-1) + a(n-2) - 4*a(n-3) + 4*a(n-4) - 2*a(n-5)
   Initial values (n = 1 … 13):
      a(1)=367, a(2)=562, a(3)=1203, a(4)=3186, a(5)=4875,
      a(6)=9834, a(7)=14883, a(8)=31866, a(9)=51057,
      a(10)=105398, a(11)=174289, a(12)=347428, a(13)=588058
*/
#include <stdlib.h>   /* malloc, realloc, abort */
#include <stddef.h>   /* size_t */

static seq_t *cache      = NULL;      /* memoised values (index 0 unused) */
static idx_t cache_size  = 0;         /* allocated size of cache array          */
static idx_t max_computed = 0;        /* highest n already stored (0 if none)     */

/* Initial values stored in a zero-indexed array; index 0 is unused. */
static const seq_t init_vals[14] = {
    0ULL,          /* dummy for index 0 */
    367ULL,        /* a(1) */
    562ULL,        /* a(2) */
    1203ULL,       /* a(3) */
    3186ULL,       /* a(4) */
    4875ULL,       /* a(5) */
    9834ULL,       /* a(6) */
    14883ULL,      /* a(7) */
    31866ULL,      /* a(8) */
    51057ULL,      /* a(9) */
    105398ULL,     /* a(10) */
    174289ULL,     /* a(11) */
    347428ULL,     /* a(12) */
    588058ULL      /* a(13) */
};

seq_t a(idx_t n)
{
    /* Ensure at least one slot for index 0 and all initial values.  */
    size_t needed = (size_t)(n + 1u);
    if (needed < 14u) needed = 14u;   /* guarantee space for init_vals */

    if (cache_size < needed)
    {
        size_t new_size = cache_size ? cache_size * 2u : needed;
        if (new_size < needed) new_size = needed;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();          /* realloc failed */
        cache = tmp;
        cache_size = new_size;
    }

    /* Initialise the initial segment only once. */
    if (max_computed < 13 && cache_size >= 14)
    {
        for (idx_t i = 1; i <= 13; ++i)
            cache[(size_t)i] = init_vals[(size_t)i];
        max_computed = 13;
    }

    if (n <= 13)
        return cache[(size_t)n];

    /* Compute missing values up to n using the recurrence. */
    for (idx_t i = max_computed + 1; i <= n; ++i)
    {
        seq_t val = cache[(size_t)(i-1)] * (seq_t)2
                  + cache[(size_t)(i-2)]
                  - cache[(size_t)(i-3)] * (seq_t)4
                  + cache[(size_t)(i-4)] * (seq_t)4
                  - cache[(size_t)(i-5)] * (seq_t)2;
        cache[(size_t)i] = val;
    }
    max_computed = n;

    return cache[(size_t)n];
}
