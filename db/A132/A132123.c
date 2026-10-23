/*
 * A132123  a(n) = n * (2*n + 1) * (6*n^2 + 4*n + 1) / 3.
 * https://oeis.org/A132123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2*n + 1)*(6*n^2 + 4*n + 1)/3
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 179, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* convert the index to the sequence type */
    seq_t nn = (seq_t)n;

    /* Intermediate calculations are kept small enough to avoid overflow
       for all supported seq_t sizes (up to 128 bits). */
    seq_t part1 = nn * (2 * nn + 1);
    seq_t part2 = (6 * nn * nn) + (4 * nn) + 1;

    /* Final result, division by 3 is exact by the theorem. */
    return (part1 * part2) / 3;
}
