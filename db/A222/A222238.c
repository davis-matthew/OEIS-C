/*
 * A222238  In the number n, replace all (decimal) digits '3' with '8' and
 *          vice versa.
 * https://oeis.org/A222238
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = replace all decimal digits 3 and 8 with each other
 *            (digits 3 ↔ 8) in the decimal representation of n
 * Range:     u8 n <= 229, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Special case for n = 0 */
    if (n == 0)
        return (seq_t)0;
    seq_t res   = 0;
    seq_t power = 1;
    idx_t tmp   = n;
    while (tmp != 0) {
        idx_t digit = tmp % 10;
        if (digit == 3)
            digit = 8;
        else if (digit == 8)
            digit = 3;
        res += (seq_t)digit * power;
        power *= 10;
        tmp /= 10;
    }
    return res;
}
