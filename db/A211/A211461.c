/*
 * A211461  Number of -2..2 arrays x(i) of n+1 elements i=1..n+1 with
 *          x(i)+x(j), x(i+1)+x(j+1), -(x(i)+x(j+1)), and -(x(i+1)+x(j))
 *          having two or three distinct values for every i<=n and j<=n.
 * https://oeis.org/A211461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of -2..2 arrays x[1..n+1] satisfying the * given
 *            conditions. * Empirical recurrence (valid for n >= 12): * a(n) =
 *            4*a(n-1) + 3*a(n-2) + 11*a(n-4) + 67*a(n-5) + 52*a(n-8) +
 *            18*a(n-9) * - 28*a(n-3) - 49*a(n-6) - 63*a(n-7) - 12*a(n-10) *
 *            Initial values (a(1) .. a(11)): * 24, 56, 128, 2...
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_values[11] = {
    /* a(1) .. a(11) */
    24, 56, 128, 254, 516, 982, 1904, 3578, 6812, 12786, 24224
};

static seq_t *cache = NULL;   /* 1-based values stored at cache[0] .. */
static idx_t  cache_size = 0; /* number of terms stored in cache */

/* Return the a(n) term (n >= 1).  Results are memoised. */
seq_t a(idx_t n)
{
    if (n == 0) { /* undefined, return 0 as sentinel */
        return 0;
    }

    /* initialise cache if first call */
    if (cache == NULL) {
        cache_size = 11;
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        for (idx_t i = 0; i < cache_size; ++i)
            cache[i] = init_values[i];
    }

    /* If the requested term is already computed, return it */
    if (n <= cache_size)
        return cache[n - 1];

    /* Expand cache to contain terms up to n */
    cache = (seq_t*)realloc(cache, n * sizeof(seq_t));

    /* Compute missing terms progressively */
    for (idx_t i = cache_size; i < n; ++i) {
        /* Index i corresponds to term a(i+1) */
        seq_t val = 4 * cache[i - 1];
        val += 3 * cache[i - 2];
        val += 11 * cache[i - 4];
        val += 67 * cache[i - 5];
        val += 52 * cache[i - 8];
        val += 18 * cache[i - 9];
        val -= 28 * cache[i - 3];
        val -= 49 * cache[i - 6];
        val -= 63 * cache[i - 7];
        val -= 12 * cache[i -10];
        cache[i] = val;
    }

    cache_size = n;
    return cache[n - 1];
}
