/*
 * A002676  Denominators of coefficients for central differences M_{4}^(2*n).
 * https://oeis.org/A002676
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator((2*4^n - 8)/(2n)!), i.e. the denominator of
 *            the coefficient of x^(2n) in (2*sinh(x/2))^4 = e^(2x) - 4e^x + 6
 *            - 4e^(-x) + e^(-2x). Computed prime by prime: v_p(a(n)) = max(0,
 *            v_p((2n)!) - v_p(8*(4^(n-1)-1))), using v_p(4^m-1) =
 *            v_p(4^(p-1)-1) + v_p(m) for odd p dividing 4^m-1 (LTE)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   a2676_mulmod, a2676_powmod, a2676_vnum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/a2676_mulmod.h"
#include "../utils/a2676_powmod.h"
#include "../utils/a2676_vnum.h"

seq_t a(idx_t n)
{
    idx_t N = 2 * n;
    idx_t m = n - 1;
    unsigned char *comp;
    seq_t result = 1;
    uint64_t chunk = 1;
    idx_t i, j;

    comp = (unsigned char *)calloc((size_t)N + 1, 1);
    for (i = 2; i <= N; i++) {
        uint64_t ef, en, e, k;
        idx_t q;
        if (comp[i]) continue;
        for (j = i * i; j <= N; j += i) comp[j] = 1;

        ef = 0;
        q = N;
        while (q) {
            q /= i;
            ef += q;
        }
        if (i == 2) en = 3;
        else en = a2676_vnum(i, m);
        if (ef <= en) continue;
        e = ef - en;
        for (k = 0; k < e; k++) {
            if (chunk > ((uint64_t)1 << 40)) {
                result *= (seq_t)chunk;
                chunk = 1;
            }
            chunk *= i;
        }
    }
    result *= (seq_t)chunk;
    free(comp);
    return result;
}
