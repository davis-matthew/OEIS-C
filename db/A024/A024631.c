/*
 * A024631  n written in fractional base 4/3.
 * https://oeis.org/A024631
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the integer obtained by writing n in fractional base 4/3
 *            and interpreting the resulting digit string as a base‑10 number
 * Range:     u8 n <= 7, u16 n <= 23, u32 n <= 107, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)      /* a(0) = 0 */
        return 0;

    const idx_t base   = 4;   /* numerator a in 4/3 */
    const idx_t carry  = 3;   /* denominator b in 4/3 */

    idx_t temp = n;
    unsigned char digits[128];   /* enough for n up to 10⁴ (≈75 digits) */
    size_t len = 0;

    /* Convert n to base 4/3 with digits 0..3, using the recurrence
       n_{k+1} = (n_k / 4) * 3, d_k = n_k % 4.                      */
    while (temp > 0) {
        digits[len++] = temp % base;
        temp = (temp / base) * carry;
    }

    /* Assemble the digits into the final decimal value.          */
    seq_t result = 0;
    for (size_t i = 0; i < len; ++i)
        result = result * 10 + digits[len - 1 - i];

    return result;
}
