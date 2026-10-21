/*
 * A212501  Number of (w,x,y,z) with all terms in {1,...,n} and w > x < y >=
 *          z.
 * https://oeis.org/A212501
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)*n*(n+1)*(5*n-2)/24 (OEIS A212501)
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n = 0 or 1 the formula gives 0 automatically, but the
       subtraction below would underflow for unsigned types.  Treat
       these cases explicitly. */
    if (n < 2)
        return (seq_t)0;

    seq_t N = (seq_t)n;

    /* Compute (N-1) * N * (N+1) * (5*N-2) / 24 using integer arithmetic. */
    seq_t expr = (N - 1) * N * (N + 1) * ((seq_t)5 * N - (seq_t)2);
    return expr / (seq_t)24;
}
