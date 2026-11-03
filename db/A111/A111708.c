/*
 * A111708  a(n) = n concatenated with 9's complement of n.
 * https://oeis.org/A111708
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = (n + 1) * (10^k - 1), where k is the decimal digit count
 *            of n
 * Range:     u8 n <= 9, u16 n <= 99, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Validated: standard, pass, 62 terms through n = 8724 (2026-11-01)
 * Helpers:   num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/num_digits_idx.h"

static seq_t pow10(idx_t exp) {
    seq_t res = 1;
    for (idx_t i = 0; i < exp; ++i) res *= (seq_t)10;
    return res;
}

seq_t a(idx_t n)
{
    /* Determine the number of decimal digits of n. */
    int k = num_digits_idx(n);          /* count_digits(0) returns 1 */

    /* Compute 10^k - 1 */
    seq_t ten_pow = pow10((idx_t)k);
    seq_t mult    = ten_pow - 1;

    /* a(n) = (n + 1) * (10^k - 1) */
    return ((seq_t)n + 1) * mult;
}
