/*
 * A055658  Number of (3,n)-partitions of a chain of length n^2.
 * https://oeis.org/A055658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/6)*(n-1)*(n-2)*(n^2-3*n+3)*(n^2-3*n+1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 54, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n = 1 or 2 the expression contains a zero factor, so the
       result is 0.  Avoid unsigned underflow in the (n-2) term by
       handling these cases explicitly. */
    if (n <= 2)
        return 0;

    seq_t N = (seq_t)n;   /* promote to the type used for sequence values */

    seq_t t1 = (N - 1) * (N - 2);          /* (n-1)(n-2)                */
    seq_t t2 = N * N - (seq_t)3 * N + (seq_t)3;  /* n^2 - 3n + 3              */
    seq_t t3 = N * N - (seq_t)3 * N + (seq_t)1;  /* n^2 - 3n + 1              */

    seq_t result = t1 * t2 * t3;           /* product of four terms      */
    result /= (seq_t)6;                    /* divide by 6 (exact)        */
    return result;
}
