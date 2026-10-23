/*
 * A171722  a(n) = 0+1+2+...+n in lunar arithmetic in base 9 written in base
 *          10.
 * https://oeis.org/A171722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = digitwise maximum of 0..n in base 9 (lunar arithmetic)
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    seq_t power = 1;

    while (power <= (seq_t)n) {
        /* digit at current position */
        seq_t digit = ((seq_t)n / power) % 9;

        /* if a number with digit 8 is available at this position */
        if ((seq_t)n / 8 >= power) {
            digit = 8;
        }

        res += digit * power;

        /* avoid overflow when increasing power */
        if (power > ((seq_t)n) / 9) {
            break;
        }
        power *= 9;
    }

    return res;
}
