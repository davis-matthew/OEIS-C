/*
 * A037124  Numbers that contain only one nonzero digit.
 * https://oeis.org/A037124
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (((n - 1) mod 9) + 1) * 10^floor((n - 1)/9)
 * Range:     u8 n <= 20, u16 n <= 42, u32 n <= 85, u64 n <= 172,
 *            u128 n <= 345, big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* The offset of the sequence is 1; for n == 0 the result is undefined.
       We return 0 for safety. */
    if (n == 0) {
        return 0;
    }

    idx_t m = n - 1;
    idx_t digit = (m % 9) + 1;        /* value in [1,9] */
    idx_t exp   = m / 9;              /* non‑negative exponent */

    seq_t pow_val = pow10((idx_t)exp); /* 10^exp as seq_t */

    return (seq_t)digit * pow_val;
}
