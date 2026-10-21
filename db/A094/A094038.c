/*
 * A094038  Binomial transform of (Pell(-n)+Pell(n))/2.
 * https://oeis.org/A094038
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*a(n-1) - 8*a(n-3) + 4*a(n-4), with a(0)=0, a(1)=1,
 *            a(2)=2, a(3)=8
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n-th term of OEIS A094038 (binomial transform of (Pell(-n)+Pell(n))/2). */
seq_t a(idx_t n)
{
    /* Handle the base cases explicitly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)8;

    /* Variables holding the last four computed terms. */
    seq_t a_nm4 = (seq_t)0; /* a(n-4)  */
    seq_t a_nm3 = (seq_t)1; /* a(n-3)  */
    seq_t a_nm2 = (seq_t)2; /* a(n-2)  */
    seq_t a_nm1 = (seq_t)8; /* a(n-1)  */
    seq_t a_curr = (seq_t)0;/* current term */

    /* Iterate from n=4 up to the requested index. */
    for (idx_t i = 4; i <= n; ++i) {
        /* a(i) = 4*a(i-1) - 8*a(i-3) + 4*a(i-4) */
        a_curr = 4 * a_nm1 + 4 * a_nm4 - 8 * a_nm3;

        /* Shift the window of stored terms. */
        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_curr;
    }
    return a_curr;
}
