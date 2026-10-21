/*
 * A224000  Number of 2 X n 0..2 arrays with antidiagonals unimodal and rows
 *          and diagonals nondecreasing.
 * https://oeis.org/A224000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^4 + 12*n^3 + 41*n^2 + 42*n + 12) / 12
 * Range:     u8 n <= 1, u16 n <= 13, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; a(0) is unused. */
    seq_t sn = (seq_t)n;
    seq_t n2 = sn * sn;          /* n² */
    seq_t n3 = n2 * sn;          /* n³ */
    seq_t n4 = n3 * sn;          /* n⁴ */
    seq_t num = n4
              + (seq_t)12 * n3
              + (seq_t)41 * n2
              + (seq_t)42 * sn
              + (seq_t)12;
    return num / (seq_t)12;
}
