/*
 * A061524  Surround numbers of an n X 2 rectangle when n is even.
 * https://oeis.org/A061524
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5*a(n-1) - 10*a(n-2) + 10*a(n-3) - 5*a(n-4) + a(n-5), n >
 *            6 Initial values: a(2) = 224 a(3) = 705 a(4) = 1600 a(5) = 3065
 *            a(6) = 5280
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n < 2) return 0;

    static seq_t *cache = NULL;
    static idx_t cache_size = 0;  /* number of allocated slots (indices 0..cache_size-1) */

    if (cache_size <= n) {
        idx_t new_size = n + 1;
        seq_t *old = cache;
        cache = (seq_t *)realloc(old, new_size * sizeof(seq_t));
        if (!cache) return 0;  /* out‑of‑memory: return 0 */

        /* Fill newly allocated entries */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i < 2) {
                cache[i] = 0;
            } else if (i == 2) {
                cache[2] = 224;
            } else if (i == 3) {
                cache[3] = 705;
            } else if (i == 4) {
                cache[4] = 1600;
            } else if (i == 5) {
                cache[5] = 3065;
            } else if (i == 6) {
                cache[6] = 5280;
            } else {
                /* recurrence computation: avoid underflow in unsigned arithmetic */
                seq_t t1 = 5 * cache[i-1];
                seq_t t2 = 10 * cache[i-2];
                seq_t t3 = 10 * cache[i-3];
                seq_t t4 = 5 * cache[i-4];
                seq_t t5 = cache[i-5];

                seq_t sum = t1 + t3 + t5;
                sum -= t2;
                sum -= t4;
                cache[i] = sum;
            }
        }
        cache_size = new_size;
    }
    return cache[n];
}
