/*
 * A103725  Column m=8 sequence of triangle A103718(n,m), n>=0, without
 *          leading zeros.
 * https://oeis.org/A103725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = |S1(n+9,9)| + |S1(n+9,10)| where S1 is the unsigned
 *            Stirling number of the first kind. The function computes the
 *            triangle entries for k=9 and k=10 up to row n+10 using the
 *            recurrence S1(N,k) = S1(N-1,k-1) + (N-1)*S1(N-1,k)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* largest row needed is n+10; we only care about k<=10 */
    idx_t N1   = n + 9;      /* row for S1(n+9,9) and S1(n+9,10)   */
    idx_t N2   = n + 10;     /* row for S1(n+10,10)               */
    idx_t Nmax = N2;         /* maximal row we must compute        */

    /* s[k] will hold S1(current_row, k)   */
    seq_t s[11] = {};         /* zero‑initialize all entries       */
    s[0] = (seq_t)1;          /* S1(0,0) = 1                       */

    seq_t s_n9_9   = (seq_t)0;        /* will store S1(n+9,9)   */
    seq_t s_n9_10  = (seq_t)0;        /* will store S1(n+9,10)  */
    seq_t s_n10_10 = (seq_t)0;        /* will store S1(n+10,10) */

    for (idx_t N = 1; N <= Nmax; ++N)
    {
        /* update in descending order to avoid overwriting
           values that are still needed for this row       */
        for (int k = 10; k >= 1; --k)
        {
            seq_t add = (seq_t)(N - 1) * s[k];
            s[k] = s[k - 1] + add;
        }
        s[0] = (seq_t)0;   /* S1(N,0) = 0 for N >= 1              */

        if (N == N1)
        {
            s_n9_9  = s[9];
            s_n9_10 = s[10];
        }
        if (N == N2)
        {
            s_n10_10 = s[10];
        }
    }

    /* result is |S1(n+9,9)| + |S1(n+9,10)| */
    return s_n9_9 + s_n9_10;
}
