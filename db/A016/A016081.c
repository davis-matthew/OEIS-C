/*
 * A016081  Add 4, then reverse digits; start with 3.
 * https://oeis.org/A016081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 3, a(n) = rev( a(n-1) + 4 ) for n > 1
 * Range:     u8 n <= 7, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th term of OEIS A016081 (Add 4, then reverse digits). */
#include "../utils/reverse_digits.h"

seq_t a(idx_t n)
{
    /* a(1) is the first term */
    seq_t val = (seq_t)3;

    /* Iterate from the second term up to n */
    for (idx_t i = 2; i <= n; ++i) {
        val += (seq_t)4;            /* add 4 */
        val = reverse_digits(val);  /* reverse decimal digits */
    }

    return val;
}
