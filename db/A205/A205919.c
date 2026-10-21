/*
 * A205919  Number of (n+1) X 2 0..3 arrays with the number of clockwise edge
 *          increases in every 2 X 2 subblock equal to one, and every 2 X 2
 *          determinant nonzero.
 * https://oeis.org/A205919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 18*a(n-2) + 3*a(n-3) - 2*a(n-4) + 26*a(n-5) +
 *            28*a(n-6) + 8*a(n-7); Initial values: a(1)=76, a(2)=389,
 *            a(3)=1828, a(4)=9144, a(5)=43572, a(6)=215294, a(7)=1035724
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 12, u64 n <= 26, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Handle indices below offset */
    if (n == 0) return (seq_t)0;

    /* Static recurrence cache */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t have = 0;

    /* Initial values for n = 1..7 */
    static const seq_t init[7] = {
        (seq_t)76,
        (seq_t)389,
        (seq_t)1828,
        (seq_t)9144,
        (seq_t)43572,
        (seq_t)215294,
        (seq_t)1035724
    };

    /* Return directly if within initial segment */
    if (n <= 7) {
        return init[n-1];
    }

    /* Allocate cache if first call */
    if (!cache) {
        cap = 16; /* start with a small capacity */
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
    }

    /* Fill initial cache entries on first use */
    if (have < 7) {
        for (idx_t i = 1; i <= 7; ++i) {
            cache[i] = init[i-1];
        }
        have = 7;
    }

    /* Grow cache and compute needed terms */
    if (n > have) {
        /* Ensure capacity */
        if ((size_t)(n + 1) > cap) {
            size_t newcap = cap;
            while ((size_t)(n + 1) > newcap) newcap <<= 1;
            seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp) return (seq_t)0; /* allocation failure – fall back */
            cache = tmp;
            cap = newcap;
        }

        /* Compute terms from have+1 up to n */
        for (idx_t i = have + 1; i <= n; ++i) {
            seq_t val = cache[i-1];
            val += cache[i-2] * (seq_t)18;
            val += cache[i-3] * (seq_t)3;
            val -= cache[i-4] * (seq_t)2;
            val += cache[i-5] * (seq_t)26;
            val += cache[i-6] * (seq_t)28;
            val += cache[i-7] * (seq_t)8;
            cache[i] = val;
        }
        have = n;
    }

    return cache[n];
}
