/*
 * A230197  The number of multinomial coefficients over partitions with value
 *          equal to 7.
 * https://oeis.org/A230197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n-1)/6) + floor((n-1)/7) - floor(n/7)
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* undefined for n=0, safe guard */
    idx_t n1 = n - 1;
    seq_t part1 = (seq_t)n1 / 6;
    seq_t part2 = (seq_t)n1 / 7;
    seq_t part3 = (seq_t)n  / 7;
    return part1 + part2 - part3;
}
