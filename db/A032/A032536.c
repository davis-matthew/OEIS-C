/*
 * A032536  Integer part of decimal 'base-3 looking' numbers divided by their
 *          actual base-3 values.
 * https://oeis.org/A032536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = floor(A007089(n) / n)
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A007089(n) is the decimal number obtained by interpreting the base‑3
   representation of n as a decimal integer. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset is 1 in OEIS; defensive */

    /* Convert n to base‑3 digits (least significant first). */
    unsigned char digits[32];           /* max digits for 3^32 > 2^64 */
    int len = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        digits[len++] = tmp % 3;
        tmp /= 3;
    }

    /* Build the decimal integer from the digits (most significant first). */
    unsigned long long dec = 0;          /* fits all expected values */
    for (int i = len - 1; i >= 0; --i)
        dec = dec * 10 + digits[i];

    /* Integer part of dec / n. The result fits in seq_t for the
       supported build sizes. */
    return (seq_t)(dec / n);
}
