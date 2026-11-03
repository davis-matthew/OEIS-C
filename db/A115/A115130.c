/*
 * A115130  Partial sums of A005557.
 * https://oeis.org/A115130
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 42*binom(n+6,6) - 120*binom(n+5,6) + 135*binom(n+4,6) -
 *            70*binom(n+3,6) + 14*binom(n+2,6)
 * Range:     u8 n <= 1, u16 n <= 11, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Validated: partial, pass, 31 terms through n = 30 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Compute the required binomial coefficients */
    seq_t b6 = binomial(n + 6, 6);
    seq_t b5 = binomial(n + 5, 6);
    seq_t b4 = binomial(n + 4, 6);
    seq_t b3 = binomial(n + 3, 6);
    seq_t b2 = binomial(n + 2, 6);

    /* Combine them according to the generating‑function formula */
    return 42 * b6 - 120 * b5 + 135 * b4 - 70 * b3 + 14 * b2;
}
