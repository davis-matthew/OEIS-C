/*
 * A002549  Numerators of coefficients of log(1+x)/sqrt(1+x).
 * https://oeis.org/A002549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of Sum_{j=0..n-1} binomial(2j,j)/(4^j*(n-j)).
 *            This is |[x^n] log(1+x)/sqrt(1+x)|. Let L = lcm(1..n). The code
 *            forms S = Sum_j binomial(2j,j)*4^(n-1-j)*L/(n-j) with exact
 *            multiprecision arithmetic, then divides out gcd(S, 4^(n-1)*L)
 *            one prime at a time
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 14, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   a2549_add, a2549_div, a2549_mod, a2549_mul, a2549_trim
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a2549_add.h"
#include "../utils/a2549_div.h"
#include "../utils/a2549_mod.h"
#include "../utils/a2549_mul.h"
#include "../utils/a2549_trim.h"

seq_t a(idx_t n) {
    size_t cap = (size_t)(n / 8) + 16;
    uint32_t *L = (uint32_t *)calloc(cap, sizeof(uint32_t));
    uint32_t *U = (uint32_t *)calloc(cap, sizeof(uint32_t));
    uint32_t *T = (uint32_t *)calloc(cap, sizeof(uint32_t));
    uint32_t *S = (uint32_t *)calloc(cap, sizeof(uint32_t));
    unsigned char *comp = (unsigned char *)calloc((size_t)n + 1, 1);
    size_t Llen = 1, Ulen, Slen = 0, i;
    idx_t p, q, j, e2 = 0;
    uint64_t s, off, bit, tz, z;
    seq_t r = 0;

    /* L = lcm(1..n) */
    L[0] = 1;
    for (p = 2; p <= n; p++) {
        if (comp[p]) continue;
        for (q = p * p; q <= n; q += p) comp[q] = 1;
        {
            uint64_t pk = p;
            while (pk * p <= n) { pk *= p; if (p == 2) e2++; }
            if (p == 2) e2++;
            Llen = a2549_mul(L, Llen, (uint32_t)pk);
        }
    }

    /* U = L * 4^(n-1) */
    s = 2 * (uint64_t)(n - 1);
    off = s / 32; bit = s % 32;
    for (i = 0; i < Llen; i++) {
        uint64_t v = (uint64_t)L[i] << bit;
        U[i + off] |= (uint32_t)v;
        U[i + off + 1] |= (uint32_t)(v >> 32);
    }
    Ulen = a2549_trim(U, Llen + (size_t)off + 1);

    /* S = sum_j U_j/(n-j), U_j = binom(2j,j)*4^(n-1-j)*L */
    for (j = 0; j < n; j++) {
        size_t tl;
        memcpy(T, U, Ulen * sizeof(uint32_t));
        tl = a2549_div(T, Ulen, (uint32_t)(n - j));
        Slen = a2549_add(S, Slen, T, tl);
        if (j + 1 < n) {
            Ulen = a2549_mul(U, Ulen, (uint32_t)(2 * j + 1));
            Ulen = a2549_div(U, Ulen, (uint32_t)(2 * (j + 1)));
        }
    }
    Slen = a2549_trim(S, Slen);

    /* remove gcd with 4^(n-1)*L: prime 2 */
    e2 += 2 * (n - 1);
    tz = 0;
    while (tz < e2 && ((S[tz / 32] >> (tz % 32)) & 1u) == 0) tz++;
    z = tz;
    if (z > 0) {
        off = z / 32; bit = z % 32;
        for (i = 0; i + off < Slen; i++) {
            uint32_t v = S[i + off] >> bit;
            if (bit && i + off + 1 < Slen) v |= S[i + off + 1] << (32 - bit);
            S[i] = v;
        }
        for (; i < Slen; i++) S[i] = 0;
        Slen = a2549_trim(S, Slen);
    }
    /* odd primes */
    for (p = 3; p <= n; p += 2) {
        uint64_t pk;
        if (comp[p]) continue;
        for (pk = p; pk <= n; pk *= p) {
            if (a2549_mod(S, Slen, (uint32_t)p) != 0) break;
            Slen = a2549_div(S, Slen, (uint32_t)p);
        }
    }

    /* convert to seq_t */
    i = Slen;
    while (i > 0) {
        int b;
        i--;
        for (b = 3; b >= 0; b--) {
            unsigned byte = (unsigned)((S[i] >> (8 * b)) & 255u);
            r = r * 256 + (seq_t)byte;
        }
    }

    free(L); free(U); free(T); free(S); free(comp);
    return r;
}
