/*
 * A132122  a(n) = n * (n+1)^2 * (3*n^2 + 4*n + 2) / 12.
 * https://oeis.org/A132122
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)^2*(3*n^2 + 4*n + 2) / 12
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t tn  = (seq_t)n;
    seq_t n1  = tn + 1;

    /* (n+1)^2 */
    seq_t part2 = n1 * n1;

    /* 3*n^2 + 4*n + 2 */
    seq_t part3 = (seq_t)3 * tn * tn + (seq_t)4 * tn + (seq_t)2;

    seq_t result = tn * part2;      /* n * (n+1)^2 */
    result = result * part3;        /* * (3*n^2 + 4*n + 2) */
    result = result / 12;           /* final division */

    return result;
}
