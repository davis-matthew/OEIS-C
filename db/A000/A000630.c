/*
 * A000630  Number of ways to represent n using the binary operator a * b =
 *          2^a + b.
 * https://oeis.org/A000630
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = Sum_{k>=0, 2^k<=n} a(k)*a(n-2^k), a(0) = 1 (from Sum a(n)
 *            q^n = 1/(1 - Sum a(n) q^(2^n)))
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cap = 0;
    static idx_t done = 0;   /* number of computed entries */

    if (n >= cap) {
        idx_t newcap = cap ? cap : 1024;
        while (newcap <= n) newcap *= 2;
        seq_t *nc = (seq_t *)calloc((size_t)newcap, sizeof(seq_t));
        for (idx_t i = 0; i < newcap; i++) nc[i] = 0;
        for (idx_t i = 0; i < done; i++) nc[i] = cache[i];
        if (cache) free(cache);
        cache = nc;
        cap = newcap;
    }
    if (done == 0) {
        cache[0] = 1;
        done = 1;
    }
    while (done <= n) {
        idx_t i = done;
        seq_t val = 0;
        for (idx_t m = 0; m < 64 && ((idx_t)1 << m) <= i; m++) {
            val += cache[m] * cache[i - ((idx_t)1 << m)];
        }
        cache[i] = val;
        done++;
    }
    return cache[n];
}
