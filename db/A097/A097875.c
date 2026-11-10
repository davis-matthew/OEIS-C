/*
 * A097875  Denominator of J(n) = A000356(n)/A000309(n) (average number of
 *          4-colorings of rank 0 in a rooted nonseparable map which is
 *          trivalent and has 2n nodes).
 * https://oeis.org/A097875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of J(n) = A000356(n)/A000309(n), with
 *            A000356(n) = (2n)!(2n+1)!/(n!^2 (n+1)! (n+2)!), A000309(n) = 2^n
 *            (3n)!/((n+1)!(2n+1)!); computed as the reduced fraction r(1) =
 *            1/1, r(i) = r(i-1) * 4(2i-1)(2i+1)^2 / (3(i+2)(3i-1)(3i-2))
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Validated: partial, pass, 24 terms through n = 24 (2026-11-07)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    seq_t p = 1, q = 1;
    for (idx_t i = 2; i <= n; i++) {
        seq_t num = (seq_t)4 * ((seq_t)2 * i - 1) * ((seq_t)2 * i + 1) * ((seq_t)2 * i + 1);
        seq_t den = (seq_t)3 * ((seq_t)i + 2) * ((seq_t)3 * i - 1) * ((seq_t)3 * i - 2);
        seq_t g = gcd(num, den);
        num /= g;
        den /= g;
        g = gcd(p, den);
        p /= g;
        den /= g;
        g = gcd(num, q);
        num /= g;
        q /= g;
        p = p * num;
        q = q * den;
    }
    return q;
}
