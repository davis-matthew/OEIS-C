/*
 * A145068  Zero followed by partial sums of A059100, starting at n=1.
 * https://oeis.org/A145068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)*(2*n^2 - n + 12)/6
 * Range:     u8 n <= 5, u16 n <= 32, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For safety, handle n == 0 (outside the sequence's offset). */
    if (n == 0)
        return 0;

    seq_t N  = n;                   /* Convert index to seq_t            */
    seq_t N1 = N - 1;                /* (n-1)                              */
    seq_t term = ((seq_t)2 * (N * N)) - N + (seq_t)12; /* 2*n^2 - n + 12 */
    seq_t prod = N1 * term;          /* (n-1) * (2*n^2 - n + 12)          */
    return prod / (seq_t)6;          /* Divide by 6 to get the result      */
}
