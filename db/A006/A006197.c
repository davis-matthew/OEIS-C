/*
 * A006197  Least number not dividing binomial(2n,n).
 * https://oeis.org/A006197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A007978(A000984(n)): search for the least prime power p^e
 *            with v_p(binomial(2n,n)) < e, where v_p(binomial(2n,n)) =
 *            Sum_{i>=1} (floor(2n/p^i) - 2*floor(n/p^i)) (Legendre/Kummer)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 87 terms through n = 9958 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t vp_central(uint64_t n, uint64_t p) {
    uint64_t v = 0;
    uint64_t q = p;
    while (q <= 2 * n) {
        v += (2 * n) / q - 2 * (n / q);
        if (q > (2 * n) / p) break;
        q *= p;
    }
    return v;
}

seq_t a(idx_t n) {
    uint64_t k;
    for (k = 2;; k++) {
        uint64_t p = 0, m = k, e = 0, d;
        for (d = 2; d * d <= m; d++) {
            if (m % d == 0) { p = d; break; }
        }
        if (p == 0) p = m;
        while (m % p == 0) { m /= p; e++; }
        if (m != 1) continue; /* not a prime power */
        if (vp_central((uint64_t)n, p) < e) return (seq_t)k;
    }
}
