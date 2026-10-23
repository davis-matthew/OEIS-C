/*
 * A166830  Number of n X 3 1..2 arrays containing at least one of each value,
 *          all equal values connected, rows considered as a single number in
 *          nondecreasing order, and columns considered as a single number in
 *          nonincreasing order.
 * https://oeis.org/A166830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 + 6*n^2 + 11*n - 6) / 6
 * Range:     u8 n <= 4, u16 n <= 38, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For the offset 1 the sequence is undefined for n = 0.
       We return 0 for safety though the caller should not request n = 0. */
    if (n == 0) return 0;

    seq_t sn  = (seq_t)n;          /* n */
    seq_t sn2 = sn * sn;           /* n^2 */
    seq_t sn3 = sn2 * sn;          /* n^3 */

    seq_t num = sn3
                + (seq_t)6 * sn2
                + (seq_t)11 * sn
                - (seq_t)6;

    return num / (seq_t)6;
}
