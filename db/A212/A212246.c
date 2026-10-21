/*
 * A212246  Number of (w,x,y,z) with all terms in {1,...,n} and w <= x > y <=
 *          z.
 * https://oeis.org/A212246
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)*n*(n+1)*(5*n+6)/24
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n=0 and n=1 the formula would underflow the unsigned type
       (n-1 would wrap around).  The known values are 0, so handle
       these cases explicitly. */
    if (n < 2)
        return 0;

    seq_t N = (seq_t)n;
    seq_t t1 = N - 1;          /* n-1 */
    seq_t t2 = N;              /* n   */
    seq_t t3 = N + 1;          /* n+1 */
    seq_t t4 = 5 * N + 6;      /* 5*n+6 */

    seq_t num = t1 * t2;
    num = num * t3;
    num = num * t4;

    return num / 24;
}
