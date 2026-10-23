/*
 * A135295  a(n) = n^(number of decimal digits of n).
 * https://oeis.org/A135295
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = n ^ (number of decimal digits of n)
 * Range:     u8 n <= 15, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   big_pow, digit_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_count.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    /* a(0) is defined as 0 */
    if (n == 0) return 0;

    /* Count decimal digits of n (n > 0). */
    uint64_t d = digit_count(n);

    /* Compute n^d using the big_pow helper. */
    return big_pow((seq_t)n, (seq_t)d);
}
