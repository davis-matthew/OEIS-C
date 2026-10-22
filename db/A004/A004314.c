/*
 * A004314  a(n) = binomial coefficient C(2n, n - 8).
 * https://oeis.org/A004314
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(2n, n-8), computed multiplicatively as C(2n,k) =
 *            Prod_{i=1..k} (2n-k+i)/i with k = n-8, dividing out gcds first
 *            so intermediates never exceed a(n)
 * Range:     u8 n <= 10, u16 n <= 12, u32 n <= 19, u64 n <= 35, u128 n <= 66,
 *            big any n
 * Helpers:   a004314_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a004314_gcd.h"

seq_t a(idx_t n) {
    idx_t k = n - 8;
    seq_t r = 1;
    idx_t i;
    for (i = 1; i <= k; i++) {
        seq_t num = (seq_t)(n + 8 + i);
        seq_t den = (seq_t)i;
        seq_t g = a004314_gcd(r, den);
        r /= g;
        den /= g;
        /* now den divides num, since gcd(r, den) == 1 */
        r *= num / den;
    }
    return r;
}
