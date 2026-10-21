/*
 * A202455  Number of (n+2) X 4 binary arrays with consecutive windows of
 *          three bits considered as a binary number nondecreasing in every
 *          row and column.
 * https://oeis.org/A202455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^4 + 30*n^3 + 229*n^2 + 948*n + 1708) / 4
 * Range:     u8 none, u16 n <= 9, u32 n <= 210, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;           /* safety for n < 1 */
    /* Convert the index to the sequence type */
    seq_t sn = (seq_t)n;
    seq_t n2 = sn * sn;                    /* n^2 */
    seq_t n3 = n2 * sn;                    /* n^3 */
    seq_t n4 = n3 * sn;                    /* n^4 */

    /* numerator = n^4 + 30*n^3 + 229*n^2 + 948*n + 1708 */
    seq_t num = n4
            + (seq_t)30 * n3
            + (seq_t)229 * n2
            + (seq_t)948 * sn
            + (seq_t)1708;

    /* The denominator is 4, division is exact for integer n ≥ 1 */
    return num / (seq_t)4;
}
