/*
 * A001617  Genus of modular group Gamma_0(n). Or, genus of modular curve
 *          X_0(n).
 * https://oeis.org/A001617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 1 + A001615(n)/12 - A000089(n)/4 - A000086(n)/3 -
 *            A001616(n)/2, with A001615 = psi(n) = n*Prod_{p|n}(1+1/p),
 *            A000089 = nu2 = [4 !| n]*Prod_{p|n}(1+(-4/p)), A000086 = nu3 =
 *            [9 !| n]*Prod_{p|n}(1+(-3/p)), A001616 = Sum_{d|n}
 *            phi(gcd(d,n/d))
 * Range:     u8 n <= 1259, u16 n <= 9432, u32 n <= 9432, u64 n <= 9432,
 *            u128 n <= 9432, big any n
 * Helpers:   a1617_gcd, a1617_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a1617_gcd.h"
#include "../utils/a1617_phi.h"

seq_t a(idx_t n) {
    uint64_t m = n;
    uint64_t psi = n;
    long long nu2 = (n % 4 == 0) ? 0 : 1;
    long long nu3 = (n % 9 == 0) ? 0 : 1;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            while (m % p == 0) m /= p;
            psi = psi / p * (p + 1);
            if (p != 2) nu2 *= (p % 4 == 1) ? 2 : 0;
            if (p != 3) nu3 *= (p % 3 == 1) ? 2 : 0;
        }
    }
    if (m > 1) {
        uint64_t p = m;
        psi = psi / p * (p + 1);
        if (p != 2) nu2 *= (p % 4 == 1) ? 2 : 0;
        if (p != 3) nu3 *= (p % 3 == 1) ? 2 : 0;
    }

    uint64_t cusps = 0;
    for (uint64_t d = 1; d * d <= n; d++) {
        if (n % d == 0) {
            uint64_t e = n / d;
            uint64_t ph = a1617_phi(a1617_gcd(d, e));
            cusps += ph;
            if (d != e) cusps += ph;
        }
    }

    uint64_t num = 12 + psi - 3 * (uint64_t)nu2 - 4 * (uint64_t)nu3 - 6 * cusps;
    return (seq_t)(num / 12);
}
