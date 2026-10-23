/*
 * A183857  a(n) = n - 1 + ceiling((2/3)*n^2); complement of A183874.
 * https://oeis.org/A183857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - 1 + ceil((2/3)*n^2) which is equivalent to a(n) = n -
 *            1 + (2*n*n + 2)/3 Offset: 1 (so a(1) is the first term)
 * Range:     u8 n <= 11, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t to perform arithmetic in the proper type */
    seq_t nn = (seq_t)n;

    /* Compute numerator of the ceil fraction: 2*n*n + 2 */
    seq_t num = nn * nn * 2 + 2;

    /* Integer division gives the ceil value because of the +2 adjustment */
    seq_t ceil_val = num / 3;

    /* Combine according to the definition */
    return nn - 1 + ceil_val;
}
