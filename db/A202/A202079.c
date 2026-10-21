/*
 * A202079  Number of arrays of 8 integers in -n..n with sum zero and the sum
 *          of every adjacent pair being odd.
 * https://oeis.org/A202079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) -28*a(n-2) +56*a(n-3) -70*a(n-4) +56*a(n-5)
 *            -28*a(n-6) +8*a(n-7) -a(n-8) Initial values: a(1)=12, a(2)=524,
 *            a(3)=5832, a(4)=34632, a(5)=142692, a(6)=462436, a(7)=1264272,
 *            a(8)=3044496
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 23, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base values for 1 <= n <= 8 */
    static const seq_t base[9] = {
        0ULL,          /* unused, 0 index */
        12ULL,         /* a(1) */
        524ULL,        /* a(2) */
        5832ULL,       /* a(3) */
        34632ULL,      /* a(4) */
        142692ULL,     /* a(5) */
        462436ULL,     /* a(6) */
        1264272ULL,    /* a(7) */
        3044496ULL     /* a(8) */
    };

    /* Handle small n directly */
    if (n < 9) {
        return base[n];
    }

    /* Static cache for previously computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Initial cache population */
    if (cache_size == 0) {
        cache_size = 8;
        cache = (seq_t *)malloc((cache_size + 1) * sizeof(seq_t));
        if (!cache) {
            /* In a real program we might handle this error; for the
               purpose of this function we simply return 0. */
            return 0;
        }
        cache[0] = 0;               /* Unused index 0 */
        for (idx_t i = 1; i <= 8; ++i) {
            cache[i] = base[i];
        }
    }

    /* Extend cache if necessary */
    if (n > cache_size) {
        seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!new_cache) {
            /* Allocation failed; return 0 as fallback. */
            return 0;
        }
        cache = new_cache;
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            /* Compute a(i) using the recurrence.
               All intermediate sums are larger than their
               subtractions, so unsigned arithmetic is safe. */
            seq_t pos = 8 * cache[i - 1]
                      + 56 * cache[i - 3]
                      + 56 * cache[i - 5]
                      + 8 * cache[i - 7];
            seq_t neg = 28 * cache[i - 2]
                      + 70 * cache[i - 4]
                      + 28 * cache[i - 6]
                      + cache[i - 8];
            cache[i] = pos - neg;
        }
        cache_size = n;
    }

    return cache[n];
}
