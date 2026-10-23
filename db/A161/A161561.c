/*
 * A161561  The smallest number larger than n with digital sum equal to n.
 * https://oeis.org/A161561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = min{k>n : digit_sum(k) = n}
 * Range:     u8 n <= 19, u16 n <= 41, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: build the minimal integer whose digits sum to n and is > n. */
/* For n ≤9, the next such integer is simply n+9. For n≥10, the minimal
   integer with digit sum n is obtained by writing n = 9*q + r (0 ≤ r <9)
   and forming the number r (if r≠0) followed by q copies of the digit 9.
   This number is always > n when n≥10. */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Handle the small values explicitly. */
    if (n <= 9) {
        return (seq_t)(n + 9);
    }

    /* n = 9*q + r  with 0 ≤ r < 9 */
    idx_t q = n / 9;
    idx_t r = n % 9;

    seq_t result = 0;
    if (r != 0) {
        result = (seq_t)r;
    }

    /* Append q digits of 9. */
    for (idx_t i = 0; i < q; ++i) {
        result = result * (seq_t)10 + (seq_t)9;
    }

    return result;
}
