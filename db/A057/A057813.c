/*
 * A057813  a(n) = (2*n+1)*(4*n^2+4*n+3)/3.
 * https://oeis.org/A057813
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n+1)*(4*n^2 + 4*n + 3)/3
 * Range:     u8 n <= 4, u16 n <= 28, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* convert index to seq_t first */
    seq_t nn = (seq_t)n;

    seq_t t1 = nn * 2 + 1;                     /* 2*n + 1 */
    seq_t t2 = nn * nn * 4 + nn * 4 + 3;       /* 4*n^2 + 4*n + 3 */

    /* The numerator is always divisible by 3.
       Reduce the factor that is a multiple of 3 to avoid overflow */
    if (t1 % 3 == 0)
        t1 /= 3;
    else
        t2 /= 3;

    return t1 * t2;
}
