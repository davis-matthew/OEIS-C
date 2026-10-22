/*
 * A001901  Successive numerators of Wallis's approximation to Pi/2 (reduced).
 * https://oeis.org/A001901
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = numerator(W(n)), W(n) = Product_{k=0..n} N(k)/D(k)
 *            (reduced), N(0) = 1, N(k) = 2*floor((k+1)/2) for k >= 1, D(k) =
 *            2*floor(k/2) + 1
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   wallis_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/wallis_gcd.h"

seq_t a(idx_t n) {
    seq_t q = 1;
    seq_t r = 1;

    for (idx_t k = 0; k <= n; k++) {
        seq_t numer, denom;

        if (k == 0) {
            numer = 1;
        } else if (k & 1) {
            numer = (seq_t)k + 1;
        } else {
            numer = (seq_t)k;
        }

        denom = (seq_t)(k >> 1) * 2 + 1;

        seq_t g1 = wallis_gcd(q, denom);
        q = q / g1;
        denom = denom / g1;

        seq_t g2 = wallis_gcd(r, numer);
        numer = numer / g2;
        r = r / g2;

        q = q * numer;
        r = r * denom;
    }

    return q;
}
