/*
 * A005532  Decimal expansion of fifth root of 3.
 * https://oeis.org/A005532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  cons, nonn
 * Formula:   a(n) = floor(3^(1/5) * 10^(n-1)) mod 10; 3^(1/5) = 3*y^4 where y
 *            = 3^(-1/5) is found by the Newton iteration y <- y*(6 -
 *            3*y^5)/5, using fixed-point bignums in base 10^9
 * Range:     u8 n <= 9933, u16 n <= 9933, u32 n <= 9933, u64 n <= 9933,
 *            u128 n <= 9933, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define FB 1000000000ULL

/* out = a*b in fixed point (L limbs, P fractional), tmp has 2L limbs */
static void a5532_mul(const uint64_t *a, const uint64_t *b, uint64_t *out,
                      int L, int P, uint64_t *tmp) {
    int i, j;
    memset(tmp, 0, sizeof(uint64_t) * 2 * (size_t)L);
    for (i = 0; i < L; i++) {
        uint64_t carry = 0, ai = a[i];
        if (ai == 0) continue;
        for (j = 0; j < L; j++) {
            uint64_t cur = tmp[i + j] + ai * b[j] + carry;
            tmp[i + j] = cur % FB;
            carry = cur / FB;
        }
        tmp[i + L] += carry;
    }
    for (i = 0; i < L; i++) out[i] = tmp[P + i];
}

static void a5532_mulsmall(uint64_t *a, uint64_t k, int L) {
    uint64_t carry = 0;
    int i;
    for (i = 0; i < L; i++) {
        uint64_t cur = a[i] * k + carry;
        a[i] = cur % FB;
        carry = cur / FB;
    }
}

static void a5532_divsmall(uint64_t *a, uint64_t k, int L) {
    uint64_t rem = 0;
    int i;
    for (i = L - 1; i >= 0; i--) {
        uint64_t cur = rem * FB + a[i];
        a[i] = cur / k;
        rem = cur % k;
    }
}

seq_t a(idx_t n) {
    static char *digits = NULL;
    static idx_t cached_len = 0;

    if (n > cached_len) {
        idx_t N = n;
        int P, L, i, it, iters;
        long d;
        uint64_t *y, *t, *u, *tmp;
        if (N < 2 * cached_len) N = 2 * cached_len;
        if (N < 64) N = 64;
        P = (int)((N + 30) / 9) + 2;
        L = P + 1;
        y = (uint64_t *)calloc((size_t)L, sizeof(uint64_t));
        t = (uint64_t *)calloc((size_t)L, sizeof(uint64_t));
        u = (uint64_t *)calloc((size_t)L, sizeof(uint64_t));
        tmp = (uint64_t *)calloc(2 * (size_t)L, sizeof(uint64_t));

        y[P - 1] = 802700000ULL; /* y ~ 0.8027 */
        iters = 0;
        d = 4;
        while (d < 9L * P + 20) { d *= 2; iters++; }
        iters += 2;

        for (it = 0; it < iters; it++) {
            uint64_t borrow;
            /* t = 3*y^5 */
            a5532_mul(y, y, t, L, P, tmp);      /* y^2 */
            a5532_mul(t, t, u, L, P, tmp);      /* y^4 */
            a5532_mul(u, y, t, L, P, tmp);      /* y^5 */
            a5532_mulsmall(t, 3, L);
            /* u = 6 - t */
            borrow = 0;
            for (i = 0; i < L; i++) {
                uint64_t c = (i == P) ? 6 : 0;
                uint64_t sub = t[i] + borrow;
                if (c >= sub) { u[i] = c - sub; borrow = 0; }
                else { u[i] = c + FB - sub; borrow = 1; }
            }
            /* y = y*u/5 */
            a5532_mul(y, u, t, L, P, tmp);
            a5532_divsmall(t, 5, L);
            memcpy(y, t, sizeof(uint64_t) * (size_t)L);
        }
        /* x = 3*y^4 */
        a5532_mul(y, y, t, L, P, tmp);
        a5532_mul(t, t, u, L, P, tmp);
        a5532_mulsmall(u, 3, L);

        digits = (char *)realloc(digits, (size_t)N);
        digits[0] = (char)(u[P] % 10);
        for (idx_t k = 1; k < N; k++) {
            idx_t j = k - 1;
            uint64_t limb = u[P - 1 - (int)(j / 9)];
            int pos = (int)(j % 9);
            int s;
            for (s = 0; s < 8 - pos; s++) limb /= 10;
            digits[k] = (char)(limb % 10);
        }
        cached_len = N;
        free(y); free(t); free(u); free(tmp);
    }
    return (seq_t)digits[n - 1];
}
