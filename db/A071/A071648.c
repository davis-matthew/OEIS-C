/*
 * A071648  Sum of even decimal digits of n.
 * https://oeis.org/A071648
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = sum of even decimal digits of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t x = n;
    while (x != 0) {
        unsigned d = x % 10;   /* lowest decimal digit */
        if ((d & 1) == 0) {    /* even digit? */
            sum += (seq_t)d;    /* add to sum (seq_t) */
        }
        x /= 10;               /* drop the processed digit */
    }
    return sum;
}
