/*
 * A223951  Number of 4 X n 0..1 arrays with diagonals and antidiagonals
 *          unimodal and rows nondecreasing.
 * https://oeis.org/A223951
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/3)*n^4 + (2/3)*n^3 + (37/6)*n^2 + (107/6)*n + 23
 *            (valid for n > 2; a(1)=16, a(2)=81)
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)16;
    if (n == 2) return (seq_t)81;
    /* work in seq_t to avoid overflows with large n */
    seq_t sn = (seq_t)n;
    seq_t n2 = sn * sn;         /* n^2 */
    seq_t n3 = n2 * sn;         /* n^3 */
    seq_t n4 = n3 * sn;         /* n^4 */

    /* compute numerator of the rational expression multiplied by 6:
       2*n^4 + 4*n^3 + 37*n^2 + 107*n + 138  */
    seq_t num = 2 * n4;
    num += 4 * n3;
    num += 37 * n2;
    num += 107 * sn;
    num += 138;                 /* 23 * 6 */

    return num / 6;
}
