/*
 * A024197  a(n) = 3rd elementary symmetric function of the first n+2 odd
 *          positive integers.
 * https://oeis.org/A024197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n+2)^2*(n^2+3*n+1)/6
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* convert index to seq_t once */
    seq_t nn = (seq_t)n;
    seq_t f1 = nn;                 /* n */
    seq_t f2 = nn + 1;             /* n+1 */
    seq_t f3 = nn + 2;             /* n+2 */
    seq_t f4 = f3;                 /* n+2 again */
    seq_t f5 = nn * nn + (seq_t)3 * nn + 1; /* n^2+3*n+1 */

    /* cancel factor 2 */
    if (f1 % 2 == 0)
        f1 /= 2;
    else if (f2 % 2 == 0)
        f2 /= 2;
    else if (f3 % 2 == 0)
        f3 /= 2;
    else if (f4 % 2 == 0)
        f4 /= 2;
    else
        f5 /= 2;

    /* cancel factor 3 */
    if (f1 % 3 == 0)
        f1 /= 3;
    else if (f2 % 3 == 0)
        f2 /= 3;
    else if (f3 % 3 == 0)
        f3 /= 3;
    else if (f4 % 3 == 0)
        f4 /= 3;
    else
        f5 /= 3;

    /* compute the product */
    seq_t result = f1 * f2 * f3 * f4 * f5;
    return result;
}
