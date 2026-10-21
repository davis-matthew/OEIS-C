/*
 * A210055  Number of (n+1) X 3 0..3 arrays with every 2 X 2 subblock having
 *          one, two or four distinct values, and new values 0..3 introduced
 *          in row major order.
 * https://oeis.org/A210055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 29*a(n-1) - 266*a(n-2) + 902*a(n-3) - 968*a(n-4) +
 *            176*a(n-5) + 128*a(n-6) Initial terms (offset 1): a(1)=51,
 *            a(2)=646, a(3)=8767, a(4)=124097, a(5)=1800963, a(6)=26510247
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial values */
    static const seq_t init[6] = {51, 646, 8767, 124097, 1800963, 26510247};
    /* Cache for already computed terms (1‑based index stored at position n‑1) */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of terms currently stored in cache */
    static idx_t cache_cap = 0;   /* allocated capacity in cache */

    if (n <= 6)            /* directly return the first six terms */
        return init[n-1];

    /* Ensure cache is large enough to hold at least n terms */
    if (cache_cap < n) {
        /* Grow method: double until sufficient */
        idx_t new_cap = cache_cap ? cache_cap << 1 : 64;
        while (new_cap < n) new_cap <<= 1;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Initialize cache up to term 6 on first use */
    if (cache_len == 0) {
        for (idx_t i = 0; i < 6; ++i)
            cache[i] = init[i];
        cache_len = 6;
    }

    /* Compute missing terms up to requested n */
    for (idx_t i = cache_len; i < n; ++i) {
        /* i is 0‑based index, corresponding to term (i+1) */
        seq_t t1 = 29 * cache[i-1];
        seq_t t2 = 266 * cache[i-2];
        seq_t t3 = 902 * cache[i-3];
        seq_t t4 = 968 * cache[i-4];
        seq_t t5 = 176 * cache[i-5];
        seq_t t6 = 128 * cache[i-6];
        cache[i] = t1 + t3 + t5 + t6 - t2 - t4;
    }
    cache_len = n;

    return cache[n-1];
}
