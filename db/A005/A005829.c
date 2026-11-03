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
 * Validated: partial, pass, 29 terms through n = 28 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

static int seq_is_big(void)
{
    seq_t x = 1;
    int i;
    for (i = 0; i < 130; i++) x = x * 2;
    return !(x == 0);
}

static idx_t bits_bound(idx_t i) {
    /* a(i) < 2^((9*i)/8 + 4) since a(i)/a(i-1) -> ~2.0953 < 2^(9/8) */
    return (9 * i) / 8 + 4;
}

/* floor(tau*x) via Zeckendorf: x = sum F_k  =>  tau*x = sum F_{k+1} - sum psi^k,
   so floor = shift(x) - 1 if the smallest index k is even, else shift(x). */
static seq_t ftau_zeck(seq_t x) {
    seq_t f[200];
    int m, i, kmin = -1;
    seq_t r, sh = 0;
    if (x == 0) return 0;
    f[0] = 1; /* F_2 */
    f[1] = 2; /* F_3 */
    m = 2;
    while (f[m - 1] <= x && m < 199) {
        f[m] = f[m - 1] + f[m - 2];
        m++;
    }
    r = x;
    for (i = m - 2; i >= 0; i--) {
        if (f[i] <= r) {
            r -= f[i];
            sh += f[i + 1];
            kmin = i;
        }
    }
    if (kmin >= 0 && (kmin % 2) == 0) sh -= 1;
    return sh;
}

static seq_t isqrt_big(seq_t N, idx_t startbits) {
    seq_t s = (seq_t)1 << startbits;
    for (;;) {
        seq_t t = (s + N / s) >> 1;
        if (t >= s) break;
        s = t;
    }
    return s;
}

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
        cache = mem_new(seq_t, cap);
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
        nc = mem_new(seq_t, newcap);
        for (i = 0; i < newcap; i++) nc[i] = 0;
        for (i = 0; i <= done; i++) nc[i] = cache[i];
        mem_free(cache);
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
