/*
 * A003281  Numerators of coefficients of Green function for cubic lattice.
 * https://oeis.org/A003281
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   16*n*(n+1)*(2n+1)*B1(n+1) = n*(60n^2+9)*B1(n) -
 *            3*(2n-1)^3*B1(n-1) - (n-1)*(2n-1)*(2n-3)*B1(n-2), B1(0) = 0,
 *            B1(1) = 1; a(n) = numerator(B1(n)). Computed as B1(n) =
 *            P(n)/Q(n) with Q(n) = Prod_{k=1..n-1} 16k(k+1)(2k+1) kept as
 *            prime exponents, P(n) as an exact multi-limb integer, then
 *            reduced by g...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 22,
 *            big any n
 * Helpers:   a3281_addfac, a3281_bitlen, a3281_div, a3281_mod, a3281_sub,
 *            a3281_trim
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/a3281_addfac.h"
#include "../utils/a3281_bitlen.h"
#include "../utils/a3281_div.h"
#include "../utils/a3281_mod.h"
#include "../utils/a3281_sub.h"
#include "../utils/a3281_trim.h"

typedef unsigned __int128 a3281_u128;

static size_t a3281_mul(uint64_t *out, const uint64_t *x, size_t len, uint64_t m) {
    uint64_t carry = 0;
    size_t i;
    for (i = 0; i < len; i++) {
        a3281_u128 t = (a3281_u128)x[i] * m + carry;
        out[i] = (uint64_t)t;
        carry = (uint64_t)(t >> 64);
    }
    if (carry) out[len++] = carry;
    return a3281_trim(out, len);
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;

    uint64_t qbits = 0;
    idx_t k;
    for (k = 1; k < n; k++) {
        uint64_t K = (uint64_t)k;
        qbits += (uint64_t)a3281_bitlen(16 * K * (K + 1) * (2 * K + 1));
    }
    size_t cap = (size_t)(qbits / 64 + n / 32 + 16);

    uint64_t *buf = (uint64_t *)calloc(5 * cap, sizeof(uint64_t));
    uint64_t *A = buf, *Bp = buf + cap, *C = buf + 2 * cap, *T = buf + 3 * cap, *U = buf + 4 * cap;
    size_t la = 0, lb = 0, lc = 1, lt, lu;
    C[0] = 1; /* P(1) = 1, P(0) = 0 */

    for (k = 1; k < n; k++) {
        uint64_t K = (uint64_t)k;
        lt = a3281_mul(T, C, lc, K * (60 * K * K + 9));
        if (k >= 2 && lb > 0) {
            lu = a3281_mul(U, Bp, lb, 3 * (2 * K - 1) * (2 * K - 1) * (2 * K - 1));
            lu = a3281_mul(U, U, lu, 16 * (K - 1) * K * (2 * K - 1));
            lt = a3281_sub(T, lt, U, lu);
        }
        if (k >= 3 && la > 0) {
            lu = a3281_mul(U, A, la, (K - 1) * (2 * K - 1) * (2 * K - 3));
            lu = a3281_mul(U, U, lu, 16 * (K - 1) * K * (2 * K - 1));
            lu = a3281_mul(U, U, lu, 16 * (K - 2) * (K - 1) * (2 * K - 3));
            lt = a3281_sub(T, lt, U, lu);
        }
        {
            uint64_t *fr = A;
            A = Bp; la = lb;
            Bp = C; lb = lc;
            C = T; lc = lt;
            T = fr;
        }
    }

    /* exponents of Q(n) */
    size_t M = (size_t)(2 * n + 1);
    uint32_t *spf = (uint32_t *)calloc(M + 1, sizeof(uint32_t));
    long *e = (long *)calloc(M + 1, sizeof(long));
    size_t i, j;
    for (i = 2; i <= M; i++)
        if (spf[i] == 0)
            for (j = i; j <= M; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
    for (k = 1; k < n; k++) {
        uint64_t K = (uint64_t)k;
        e[2] += 4;
        a3281_addfac(e, spf, K);
        a3281_addfac(e, spf, K + 1);
        a3281_addfac(e, spf, 2 * K + 1);
    }

    /* remove powers of 2 */
    if (lc > 0) {
        uint64_t tz = 0;
        size_t w = 0;
        while (w < lc && C[w] == 0) { tz += 64; w++; }
        { uint64_t v = C[w]; while (!(v & 1)) { tz++; v >>= 1; } }
        uint64_t s = tz < (uint64_t)e[2] ? tz : (uint64_t)e[2];
        size_t ws = (size_t)(s / 64);
        unsigned bs = (unsigned)(s % 64);
        for (i = 0; i + ws < lc; i++) {
            uint64_t lo = C[i + ws] >> bs;
            uint64_t hi = (bs && i + ws + 1 < lc) ? (C[i + ws + 1] << (64 - bs)) : 0;
            C[i] = lo | hi;
        }
        for (; i < lc; i++) C[i] = 0;
        lc = a3281_trim(C, lc);
    }

    /* remove odd prime factors common with Q */
    for (i = 3; i <= M && lc > 0; i += 2) {
        long ex = e[i];
        if (ex <= 0) continue;
        uint64_t p = (uint64_t)i, pj = p;
        long jj = 1;
        while (jj < ex && pj * p < 0xffffffffULL) { pj *= p; jj++; }
        while (ex >= jj && jj > 1) {
            if (a3281_mod(C, lc, pj) != 0) break;
            lc = a3281_div(C, lc, pj);
            ex -= jj;
        }
        while (ex > 0) {
            if (a3281_mod(C, lc, p) != 0) break;
            lc = a3281_div(C, lc, p);
            ex--;
        }
    }

    seq_t result = 0;
    i = lc;
    while (i > 0) {
        int b;
        i--;
        for (b = 7; b >= 0; b--) {
            result = (result << 8) | (seq_t)((C[i] >> (8 * b)) & 255);
        }
    }

    free(spf);
    free(e);
    free(buf);
    return result;
}
