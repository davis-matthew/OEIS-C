/*
 * A199837  Number of -n..n arrays x(0..7) of 8 elements with zero sum and no
 *          two neighbors summing to zero.
 * https://oeis.org/A199837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) - 28*a(n-2) + 56*a(n-3) - 70*a(n-4) + 56*a(n-5)
 *            - 28*a(n-6) + 8*a(n-7) - a(n-8) Initial values: a(1)=34,
 *            a(2)=6126, a(3)=113884, a(4)=888420, a(5)=4340094,
 *            a(6)=15805218, a(7)=47040968, a(8)=120843752
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 13, u64 n <= 197, u128 n <= 197,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>


seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_len = 0;  /* number of values computed, <= cache_cap */
    static size_t cache_cap = 0;  /* allocated capacity */

    if (n == 0) return 0;  /* offset starts at 1; n=0 is undefined */

    /* first allocation */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
    }

    /* ensure we have room for a(n) */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n > new_cap) new_cap <<= 1;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* return cached value if already computed */
    if ((size_t)n <= cache_len) return cache[n-1];

    /* base values */
    static const seq_t base[8] = {
        34,          /* a(1) */
        6126,        /* a(2) */
        113884,      /* a(3) */
        888420,      /* a(4) */
        4340094,     /* a(5) */
        15805218,    /* a(6) */
        47040968,    /* a(7) */
        120843752    /* a(8) */
    };

    /* compute sequentially up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t val;
        if (i <= 8) {
            val = base[i-1];
        } else {
            /* recurrence: guard against unsigned underflow by
               computing as alternating sum on unsigned values
               (the partial sums maintain non‑negative values). */
            seq_t t0 = 8 * cache[i-2];
            seq_t t1 = 28 * cache[i-3];
            seq_t t2 = 56 * cache[i-4];
            seq_t t3 = 70 * cache[i-5];
            seq_t t4 = 56 * cache[i-6];
            seq_t t5 = 28 * cache[i-7];
            seq_t t6 = 8 * cache[i-8];
            seq_t t7 = cache[i-9];
            val = t0 - t1 + t2 - t3 + t4 - t5 + t6 - t7;
        }
        cache[i-1] = val;
        if ((size_t)i > cache_len) cache_len = i;
    }

    return cache[n-1];
}
