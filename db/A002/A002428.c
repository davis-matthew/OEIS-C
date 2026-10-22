/*
 * A002428  Numerators of coefficients of expansion of arctan(x)^2 = x^2 -
 *          2/3*x^4 + 23/45*x^6 - 44/105*x^8 + 563/1575*x^10 - 3254/10395*x^12
 *          + ...
 * https://oeis.org/A002428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, frac
 * Formula:   a(n) = numerator of (-1)^n * Sum_{k=1..n-1} 1/((n-1)*(2*k-1)),
 *            for n>=1. Computed exactly as P/gcd(P, D), where L =
 *            lcm(1,3,...,2n-3), P = Sum_{k=1..n-1} L/(2k-1) and D = L*(n-1).
 *            Uses multi-limb integers internally so intermediate values
 *            cannot overflow
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 13, i64 n <= 25, i128 n <= 49,
 *            big any n
 * Helpers:   a2428_add, a2428_divq, a2428_mod, a2428_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a2428_add.h"
#include "../utils/a2428_divq.h"
#include "../utils/a2428_mod.h"
#include "../utils/a2428_mul.h"

seq_t a(idx_t n)
{
    uint64_t m, lim, p, k, nm1;
    size_t len, i, top;
    unsigned char *comp;
    uint32_t *L, *P, *T;
    seq_t r;

    if (n < 2) return 0;

    m = 2 * (uint64_t)n - 3;
    nm1 = (uint64_t)n - 1;
    lim = m > nm1 ? m : nm1;

    comp = (unsigned char *)calloc((size_t)lim + 1, 1);
    for (p = 2; p * p <= lim; p++)
        if (!comp[p])
            for (k = p * p; k <= lim; k += p) comp[k] = 1;

    len = (size_t)((n * 4) / 32 + 8);
    L = (uint32_t *)calloc(len, sizeof(uint32_t));
    P = (uint32_t *)calloc(len, sizeof(uint32_t));
    T = (uint32_t *)calloc(len, sizeof(uint32_t));

    /* L = lcm of odd numbers up to m */
    L[0] = 1;
    for (p = 3; p <= m; p += 2) {
        if (!comp[p]) {
            uint64_t pk = p;
            while (pk * p <= m) pk *= p;
            a2428_mul(L, len, (uint32_t)pk);
        }
    }

    /* P = sum L/(2k-1) */
    for (k = 1; k < (uint64_t)n; k++) {
        a2428_divq(T, L, len, (uint32_t)(2 * k - 1));
        a2428_add(P, T, len);
    }

    /* divide P by gcd(P, L*(n-1)) */
    for (p = 2; p <= lim; p++) {
        uint64_t E = 0, t;
        if (comp[p]) continue;
        if ((p & 1) && p <= m) {
            uint64_t pk = p;
            E = 1;
            while (pk * p <= m) { pk *= p; E++; }
        }
        t = nm1;
        while (t % p == 0) { t /= p; E++; }
        while (E > 0 && a2428_mod(P, len, (uint32_t)p) == 0) {
            a2428_divq(P, P, len, (uint32_t)p);
            E--;
        }
    }

    top = 0;
    for (i = 0; i < len; i++)
        if (P[i] != 0) top = i + 1;

    r = 0;
    for (i = top; i-- > 0;) {
        r = r * 65536 + (seq_t)(P[i] >> 16);
        r = r * 65536 + (seq_t)(P[i] & 0xFFFFu);
    }

    free(comp);
    free(L);
    free(P);
    free(T);

    if (n % 2 == 1) r = -r;
    return r;
}
