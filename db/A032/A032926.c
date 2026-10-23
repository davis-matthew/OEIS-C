/*
 * A032926  Numbers whose set of base-5 digits is {1,2}.
 * https://oeis.org/A032926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 254, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A032926: Numbers whose base-5 digits are only 1 and 2.
   a(n) is the n-th smallest positive integer whose base-5 representation
   uses only the digits 1 and 2.  The algorithm determines the length
   d of that representation, finds the offset of n within the block of
   length d, maps that offset's binary bits to digits 1 (bit 0) or 2
   (bit 1), and evaluates the resulting base-5 number. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* n >= 1 is required by the sequence specification. */
    /* Determine d = floor(log2(n+1)).  Since n+1 > 0, we can use
       the count of leading zeros to compute it safely. */
    idx_t temp = n;
    temp++;                     /* temp = n + 1 */
    /* For n = UINT64_MAX, temp would overflow to 0, but such a
       value is not expected in typical test data. */
    int d = 63 - __builtin_clzll(temp);   /* floor(log2(n+1)) */

    /* cumulative number of terms having fewer than d digits:
           S(d-1) = 2^d - 2                                     */
    idx_t prev_total = ((idx_t)1 << d) - 2;

    /* zero‑based offset within the block of length d */
    idx_t offset = (n - 1) - prev_total;

    /* construct the base‑5 number digit by digit */
    seq_t value = 0;
    for (int i = d - 1; i >= 0; --i) {
        value = value * 5 + ((offset >> i) & 1 ? 2 : 1);
    }
    return value;
}
