/*
 * A233068  Number of n X 3 0..3 arrays with no element x(i,j) adjacent to
 *          value 3-x(i,j) horizontally, antidiagonally or vertically, top
 *          left element zero, and 1 appearing before 2 in row major order.
 * https://oeis.org/A233068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 14*a(n-1) - 28*a(n-2) + 24*a(n-3) - 11*a(n-4) + 2*a(n-5)
 *            (n>6) Initial values: a(1)=5, a(2)=50, a(3)=582, a(4)=6854,
 *            a(5)=80811, a(6)=952869 The sequence is defined only for n≥1;
 *            a(0) is returned as 0
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Seed values of the sequence for n = 1..6 */
static const seq_t init_vals[6] =
{
    (seq_t)5,
    (seq_t)50,
    (seq_t)582,
    (seq_t)6854,
    (seq_t)80811,
    (seq_t)952869
};

/* Cache for memoized results: cache[0] is unused (index 1 maps to a(1)) */
static seq_t *cache      = NULL;
static idx_t cache_max   = 0;

/* External linkage function required by the build system */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* outside the offset – safe */

    /* initialise cache with the first six terms if needed */
    if (cache == NULL)
    {
        cache_max = 6;
        cache = (seq_t *)malloc((cache_max + 1) * sizeof(seq_t));
        if (!cache) return (seq_t)0;        /* allocation failure */
        cache[0] = (seq_t)0;                /* unused slot */
        for (idx_t i = 1; i <= cache_max; ++i)
            cache[i] = init_vals[i - 1];
    }

    if (n <= cache_max)                     /* already computed */
        return cache[n];

    /* extend the cache to hold up to n */
    size_t new_size = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) return (seq_t)0;              /* allocation failure */
    cache = tmp;
    cache[0] = (seq_t)0;                    /* keep the unused slot */

    /* compute new terms iteratively using the recurrence */
    for (idx_t i = cache_max + 1; i <= n; ++i)
    {
        seq_t val = cache[i-1] * (seq_t)14;
        val -= cache[i-2] * (seq_t)28;
        val += cache[i-3] * (seq_t)24;
        val -= cache[i-4] * (seq_t)11;
        val += cache[i-5] * (seq_t)2;
        cache[i] = val;
    }
    cache_max = n;
    return cache[n];
}
