/*
 * A095455  a(n) = 31 written in base 15 - n.
 * https://oeis.org/A095455
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = decimal integer obtained by writing the decimal value 31
 *            in base (15 - n) and interpreting the resulting digit string as
 *            a decimal number. Values with n > 14 are undefined, so return 0
 * Range:     u8 n <= 11, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n > 14) {
        return (seq_t)0;
    }

    idx_t base = 15 - n;

    if (base >= 2) {
        idx_t temp = 31;
        unsigned digits[32];
        idx_t count = 0;

        /* convert 31 to base 'base' (digits stored reversed) */
        while (temp > 0) {
            digits[count++] = temp % base;
            temp /= base;
        }

        seq_t result = 0;
        for (idx_t i = count; i-- > 0;) {
            result = result * 10 + digits[i];
        }
        return result;
    } else {            /* base == 1: unary representation of 31 */
        seq_t result = 0;
        for (idx_t i = 0; i < 31; ++i) {
            result = result * 10 + 1;
        }
        return result;
    }
}
