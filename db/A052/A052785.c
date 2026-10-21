/*
 * A052785  a(n) = 5!*n*Stirling2(n-1, 5).
 * https://oeis.org/A052785
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 120 * n * S(n-1,5)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*         where S(m,k) is the Stirling number of the second kind        */
/*         (S(0,0)=1,  S(m,0)=0 for m>0,  S(0,k)=0 for k>0)                */
/* Recurrence used for S(m,k):                                 */
/*    S(m,k) = k * S(m-1,k) + S(m-1,k-1)                        */
/* This implementation builds S(n-1,5) iteratively for each call,    */
/* resetting the row‑0 value to 0 after each step to keep the base    */
/* correct for subsequent iterations.                               */
/* Correct for all n >= 0. Elements <= 5 are zero as required.      */
seq_t a(idx_t n)
{
    /* Terms 0..5 are zero by definition. */
    if (n <= 5)
        return 0;

    /* Compute S(n-1,5) with an array of size 6. */
    seq_t s[6] = {0};
    s[0] = 1;                 /* S(0,0) = 1 */

    idx_t m = n - 1;                   /* we need S(m,5) */
    for (idx_t i = 1; i <= m; ++i) {
        /* Update in reverse to use previous row values only. */
        for (int k = 5; k >= 1; --k) {
            s[k] = (seq_t)k * s[k] + s[k - 1];
        }
        s[0] = 0;           /* for next iteration, S(i,0)=0 for i>0 */
    }

    /* a(n) = 120 * n * S(n-1,5) */
    return (seq_t)n * (seq_t)120 * s[5];
}
