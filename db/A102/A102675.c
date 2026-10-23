/*
 * A102675  Number of digits >= 5 in decimal representation of n.
 * https://oeis.org/A102675
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of digits >= 5 in the decimal representation of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 0;
    idx_t x = n;
    while (x) {
        if (x % 10 >= 5) {
            ++count;
        }
        x /= 10;
    }
    /* Handles n == 0: loop not entered, count stays 0 */
    return count;
}
