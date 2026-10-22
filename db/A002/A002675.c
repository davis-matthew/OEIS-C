/*
 * A002675  Numerators of coefficients for central differences M_{4}^(2*n).
 * https://oeis.org/A002675
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, frac
 * Formula:   (2*sinh(x/2))^4 = 2*cosh(2x) - 8*cosh(x) + 6, so the coefficient
 *            of x^(2n) is 8*(4^(n-1)-1)/(2n)!, and a(n) =
 *            numerator(8*(4^(n-1)-1)/(2n)!). The 2-part of (2n)! always
 *            cancels the 8, so a(n) = (4^(n-1)-1) / prod_{odd p<=2n}
 *            p^min(v_p(4^(n-1)-1), v_p((2n)!)), where v_p(4^(n-1)-1) =
 *            v_p(4^gcd(n-1,...
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 17, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   a2675_gcd, a2675_mul, a2675_powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/a2675_gcd.h"
#include "../utils/a2675_mul.h"
#include "../utils/a2675_powmod.h"

seq_t a(idx_t n) {
    if (n < 2) return 0;
    int fixed;
    {
        seq_t z = 0;
        z = z - 1;
        fixed = (z > 0);
    }
    idx_t m = n - 1;
    idx_t N2 = 2 * n;

    /* M = 4^m - 1 */
    seq_t r = 1, b = 4;
    idx_t e = m;
    while (e) {
        if (e & 1) r = a2675_mul(r, b);
        e >>= 1;
        if (e) b = a2675_mul(b, b);
    }
    seq_t M = r - 1;

    unsigned char *comp = (unsigned char *)calloc((size_t)N2 + 1, 1);
    if (!comp) return 0;
    for (idx_t i = 2; i * i <= N2; i++)
        if (!comp[i])
            for (idx_t j = i * i; j <= N2; j += i) comp[j] = 1;

    for (idx_t p = 3; p <= N2; p += 2) {
        if (comp[p]) continue;
        if (a2675_powmod(4, m, p) != 1) continue;
        uint64_t g = a2675_gcd(m, p - 1);
        /* largest power of p not exceeding 2^62 */
        uint64_t pk = p;
        int k = 1;
        while (pk <= ((uint64_t)1 << 62) / p) { pk *= p; k++; }
        uint64_t t = (a2675_powmod(4, g, pk) + pk - 1) % pk;
        long long v = 0;
        if (t == 0) v = k;
        else while (t % p == 0) { t /= p; v++; }
        uint64_t mm = m;
        while (mm % p == 0) { mm /= p; v++; }
        /* Legendre: v_p((2n)!) */
        long long L = 0;
        uint64_t q = N2;
        while (q) { q /= p; L += (long long)q; }
        long long ex = v < L ? v : L;
        if (ex <= 0) continue;
        if (fixed) {
            seq_t sp = (seq_t)p;
            seq_t x = sp;
            for (int it = 0; it < 8; it++) {
                seq_t tt = (seq_t)2 - a2675_mul(sp, x);
                x = a2675_mul(x, tt);
            }
            for (long long c = 0; c < ex; c++) M = a2675_mul(M, x);
        } else {
            for (long long c = 0; c < ex; c++) M = M / (seq_t)p;
        }
    }
    free(comp);
    return M;
}
