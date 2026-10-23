/*
 * A188238  Number of nondecreasing arrangements of 5 numbers in -(n+3)..(n+3)
 *          with sum zero and not more than two numbers equal.
 * https://oeis.org/A188238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-3) - 2*a(n-5) + 2*a(n-6) + a(n-8) -
 *            2*a(n-10) + a(n-11) for n > 11 (Empirical recurrence from OEIS
 *            A188238) Initial values: a(1)=58, a(2)=119, a(3)=221, a(4)=374,
 *            a(5)=598, a(6)=903, a(7)=1317, a(8)=1852, a(9)=2540, a(10)=3397,
 *            a(11)=4459
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cached sequence values, 0‑based (a(1) at index 0) */
static idx_t cache_size = 0;     /* number of entries currently in cache */

/* Predefined initial values */
static const seq_t init[11] = {
    58, 119, 221, 374, 598, 903, 1317, 1852, 2540, 3397, 4459
};

seq_t a(idx_t n)
{
    /* Return stored value if already computed */
    if (n <= cache_size)
        return cache[n - 1];

    /* Expand cache to hold values up to n */
    size_t new_size = (size_t)n;             /* n fits into size_t for practical n */
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (new_cache == NULL) {
        /* allocation failure – abort (no error reporting per requirements) */
        abort();
    }
    cache = new_cache;

    /* Fill in initial values if not already present */
    if (cache_size < 11) {
        size_t start = cache_size;
        size_t end   = (new_size < 11) ? new_size : 11;
        for (size_t i = start; i < end; ++i)
            cache[i] = init[i];
    }

    /* Compute subsequent values using the recurrence */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        if (i <= 11) {
            /* already handled by initial values */
            continue;
        }
        /* recurrence indices (converted to 0‑based) */
        seq_t a1  = cache[i - 2];
        seq_t a3  = cache[i - 4];
        seq_t a5  = cache[i - 6];
        seq_t a6  = cache[i - 7];
        seq_t a8  = cache[i - 9];
        seq_t a10 = cache[i - 11];
        seq_t a11 = cache[i - 12];

        cache[i - 1] = 2 * a1
                       - a3
                       - 2 * a5
                       + 2 * a6
                       + a8
                       - 2 * a10
                       + a11;
    }

    cache_size = new_size;
    return cache[n - 1];
}
