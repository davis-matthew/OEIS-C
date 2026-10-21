/*
 * A209597  Number of 6X6 0..n arrays with every element equal to a diagonal
 *          or antidiagonal reflection.
 * https://oeis.org/A209597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)^18 * (2*n+1)^6
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 4, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute (n+1) and (2*n+1) as seq_t */
    seq_t base1 = (seq_t)(n + 1);
    seq_t base2 = (seq_t)(2 * n + 1);

    /* compute (n+1)^18 */
    seq_t pow18 = (seq_t)1;
    for (int i = 0; i < 18; ++i)
        pow18 *= base1;

    /* compute (2*n+1)^6 */
    seq_t pow6 = (seq_t)1;
    for (int i = 0; i < 6; ++i)
        pow6 *= base2;

    /* final product */
    return pow18 * pow6;
}
