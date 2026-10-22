/*
 * A004342  Binomial coefficient C(4n, n-12).
 * https://oeis.org/A004342
 *
 * Offset:    12    (a(12) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(4n, n-12), computed multiplicatively as C(4n,k) =
 *            prod_{i=1..k} (3n+12+i)/i with k = n-12, cancelling the common
 *            factor before each multiplication
 * Range:     u8 n <= 13, u16 n <= 15, u32 n <= 19, u64 n <= 28, u128 n <= 47,
 *            big any n
 * Helpers:   gcd_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd_seq.h"

seq_t a(idx_t n)
{
    idx_t k = n - 12;
    seq_t base = (seq_t)n * 3 + 12; /* 4n - k */
    seq_t r = 1;
    idx_t i;
    for (i = 1; i <= k; i++) {
        seq_t num = base + (seq_t)i;
        seq_t den = (seq_t)i;
        seq_t g = gcd_seq(r, den);
        r /= g;
        den /= g;
        num /= den; /* exact: den divides num since gcd(r, den) = 1 */
        r *= num;
    }
    return r;
}
