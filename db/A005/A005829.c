/*
 * A005829  a(n) = floor(tau*a(n-1)) + a(n-2) with a(0)=0 and a(1)=2.
 * https://oeis.org/A005829
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(tau*a(n-1)) + a(n-2), a(0) = 0, a(1) = 2, tau =
 *            (1+sqrt(5))/2. floor(tau*x) is computed exactly with integers:
 *            via Zeckendorf shift for fixed-width builds, and via a
 *            fixed-point tau = floor(tau*2^B) for big builds
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   bits_bound, ftau_zeck, isqrt_big, seq_is_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_is_big.h"
#include "../utils/bits_bound.h"
#include "../utils/ftau_zeck.h"
#include "../utils/isqrt_big.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cap = 0;
    static idx_t done = 0;
    static seq_t T;          /* floor(tau * 2^Bt) */
    static idx_t Bt = 0;
    static int big = -1;
    idx_t i;

    if (n == 0) return 0;
    if (n == 1) return 2;

    if (big < 0) big = seq_is_big() ? 1 : 0;

    if (cache == NULL) {
        cap = (n + 1 < 64) ? 64 : n + 1;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        for (i = 0; i < cap; i++) cache[i] = 0;
        cache[0] = 0;
        cache[1] = 2;
        done = 1;
    }
    if (n <= done) return cache[n];

    if (n >= cap) {
        idx_t newcap = cap * 2;
        seq_t *nc;
        if (newcap < n + 1) newcap = n + 1;
        nc = (seq_t *)malloc(newcap * sizeof(seq_t));
        for (i = 0; i < newcap; i++) nc[i] = 0;
        for (i = 0; i <= done; i++) nc[i] = cache[i];
        free(cache);
        cache = nc;
        cap = newcap;
    }

    if (big) {
        idx_t Bneed = 2 * bits_bound(n) + 2;
        if (Bt < Bneed) {
            idx_t B = 2 * Bneed;
            seq_t N = (seq_t)5 << (2 * B);          /* 5 * 4^B */
            seq_t s = isqrt_big(N, B + 2);          /* floor(sqrt(5) * 2^B) */
            T = (((seq_t)1 << B) + s) >> 1;         /* floor(tau * 2^B) */
            Bt = B;
        }
    }

    for (i = done + 1; i <= n; i++) {
        seq_t x = cache[i - 1];
        seq_t fl;
        if (big) {
            idx_t B = 2 * bits_bound(i - 1) + 2;   /* 2^B > sqrt(5) * x^2 */
            seq_t Tb = T >> (Bt - B);              /* floor(tau * 2^B) */
            fl = (Tb * x) >> B;
        } else {
            fl = ftau_zeck(x);
        }
        cache[i] = fl + cache[i - 2];
    }
    done = n;
    return cache[n];
}
