/*
 * A046223  First denominator and then numerator of central elements of
 *          1/2-Pascal triangle.
 * https://oeis.org/A046223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A046222(n-1) for n > 5 with A046222 defined by A046222(1)
 *            = 1, A046222(3) = 1, A046222(2m) = 1 for m ≥ 3, A046222(2m+1) =
 *            Catalan(m) * (5*m + 1) / 2 for m ≥ 1 where Catalan(m) =
 *            binomial(2m, m) / (m + 1)
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 35, u64 n <= 71, u128 n <= 135,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    /* trivial initial terms */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)2;
    if (n == 4) return (seq_t)1;
    if (n == 5) return (seq_t)1;

    /* odd n > 5 always produce 1 */
    if ((n & 1) != 0) /* n is odd */
        return (seq_t)1;

    /* n is even and >= 6 */
    idx_t m = (n - 4) / 2; /* m ≥ 1 */

    /* Catalan(m) = binomial(2m, m) / (m + 1) */
    seq_t binom_val = binomial(2 * m, m);
    seq_t catalan = binom_val / (m + 1);

    seq_t mult = catalan * ( (seq_t)5 * m + 1 );
    seq_t result = mult / 2;

    return result;
}
