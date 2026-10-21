/*
 * A234277  a(n) = binomial(floor(n/2),4) +
 *          (ceiling(n/2)-3)*binomial(floor(n/2),3).
 * https://oeis.org/A234277
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(floor(n/2),4) +
 *            (ceil(n/2)-3)*binomial(floor(n/2),3)
 * Range:     u8 n <= 15, u16 n <= 50, u32 n <= 761, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t k = n / 2;          /* floor(n/2) */
    idx_t c = (n + 1) / 2;    /* ceil(n/2)  */

    seq_t result = binomial(k, 4);
    seq_t t3      = binomial(k, 3);

    if (c > 3 && t3 != 0) {
        result += (c - 3) * t3;
    }

    return result;
}
