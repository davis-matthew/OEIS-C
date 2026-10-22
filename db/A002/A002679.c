/*
 * A002679  Denominator of 2*Stirling_2(n,2)/n!.
 * https://oeis.org/A002679
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator((2^n - 2)/n!) = n!/gcd(2^n - 2, n!), computed
 *            per prime p <= n as p^max(0, v_p(n!) - v_p(2^(n-1)-1)) (v_2 of
 *            2^n-2 is 1), with v_p(2^(n-1)-1) found by lifting the exponent
 *            via the multiplicative order of 2 mod p
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 13, u64 n <= 22, u128 n <= 34,
 *            big any n
 * Helpers:   a002679_pm
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a002679_pm.h"

seq_t a(idx_t n)
{
    seq_t res = (seq_t)1;
    unsigned char *comp = (unsigned char *)malloc((size_t)n + 1);
    idx_t i, j;
    memset(comp, 0, (size_t)n + 1);
    for (i = 2; i <= n; i++) {
        uint64_t p, e, v, q, ex, k, x, d, m;
        if (comp[i]) continue;
        for (j = i * i; j <= n; j += i) comp[j] = 1;
        p = i;
        /* v_p(n!) */
        e = 0;
        q = n;
        while (q) { q /= p; e += q; }
        if (p == 2) {
            v = 1;
        } else {
            /* order of 2 mod p */
            x = 1;
            d = 0;
            for (k = 1; k <= p; k++) {
                x = x * 2 % p;
                if (x == 1) { d = k; break; }
            }
            if ((n - 1) % d != 0) {
                v = 0;
            } else {
                v = 0;
                q = p;
                for (;;) {
                    if (a002679_pm(2, d, q) != 1) break;
                    v++;
                    if (q > ((uint64_t)1 << 62) / p) break;
                    q *= p;
                }
                m = (n - 1) / d;
                while (m % p == 0) { v++; m /= p; }
            }
        }
        ex = e > v ? e - v : 0;
        for (k = 0; k < ex; k++) res *= (seq_t)p;
    }
    free(comp);
    return res;
}
