/*
 * A004315  a(n) = binomial coefficient C(2n, n-9).
 * https://oeis.org/A004315
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(2n, n-9), computed as Product_{i=1..n-9}
 *            (n+9+i)/i
 * Range:     u8 n <= 11, u16 n <= 13, u32 n <= 20, u64 n <= 35, u128 n <= 66,
 *            big any n
 * Helpers:   a004315_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a004315_gcd.h"

seq_t a(idx_t n)
{
    idx_t k = n - 9;
    seq_t r = 1;
    idx_t i;
    for (i = 1; i <= k; i++) {
        /* r = C(n+9+i, i) = C(n+8+i, i-1) * (n+9+i) / i, kept exact */
        seq_t num = (seq_t)(n + 9 + i);
        seq_t den = (seq_t)i;
        seq_t g = a004315_gcd(r, den);
        r /= g;
        den /= g;
        num /= den;
        r *= num;
    }
    return r;
}
