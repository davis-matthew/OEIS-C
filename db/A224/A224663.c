/*
 * A224663  Number of n X n 0..6 matrices with each 2X2 subblock idempotent.
 * https://oeis.org/A224663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) -12*a(n-2) +5*a(n-3) +12*a(n-4) -12*a(n-5)
 *            -3*a(n-6) +6*a(n-7) -a(n-9) a(1)=7, a(2)=28, a(3)=162, a(4)=268,
 *            a(5)=596, a(6)=1118, a(7)=2166, a(8)=4038, a(9)=7488,
 *            a(10)=13686
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    if (n < 1) return 0;
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t last_computed = 0;

    if (cache_size <= n) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (cache) {
            memcpy(tmp, cache, cache_size * sizeof(seq_t));
            free(cache);
        }
        cache = tmp;
        cache_size = new_size;
    }

    for (idx_t i = last_computed + 1; i <= n; ++i) {
        if (i == 1) cache[i] = 7;
        else if (i == 2) cache[i] = 28;
        else if (i == 3) cache[i] = 162;
        else if (i == 4) cache[i] = 268;
        else if (i == 5) cache[i] = 596;
        else if (i == 6) cache[i] = 1118;
        else if (i == 7) cache[i] = 2166;
        else if (i == 8) cache[i] = 4038;
        else if (i == 9) cache[i] = 7488;
        else if (i == 10) cache[i] = 13686;
        else {
            /* Compute with all positive terms first to avoid underflow */
            seq_t pos = 6 * cache[i-1] + 5 * cache[i-3] + 12 * cache[i-4] + 6 * cache[i-7];
            seq_t neg = 12 * cache[i-2] + 12 * cache[i-5] + 3 * cache[i-6] + cache[i-9];
            cache[i] = pos - neg;
        }
    }
    last_computed = n;
    return cache[n];
}
