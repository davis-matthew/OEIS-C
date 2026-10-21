/*
 * A091540  Rescaled second column A091539 of array A091534 ((5,2)-Stirling2).
 * https://oeis.org/A091540
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(2)=1, a(3)=13.  
 * For n>=4, let q_k = ∏_{j=0}^{k-1} (3*j+1) (q_1=1, q_{k+1}=(3*k+1)·q_k).  
 * Then a(n) - 3*n·a(n-1) = q_{n-1}.  Thus q_{n-1} = (3*n-5)·q_{n-2} and
 * a(n) = 3*n·a(n-1) + (3*n-5)·q_{n-2}.  The routine implements this
 * recurrence directly. */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;              /* undefined below offset; safe fallback */
    if (n == 2)
        return 1;              /* a(2) */
    if (n == 3)
        return 13;             /* a(3) */

    /* initialise previous two terms and q_2 = product 1·4 = 4 */
    seq_t a_nm2 = 1;           /* a(2) */
    seq_t a_nm1 = 13;          /* a(3) */
    seq_t q_nm2 = 4;           /* q_2 */

    for (idx_t cur = 4; cur <= n; ++cur) {
        /* q_{cur-1} = (3*cur-5)*q_{cur-2} */
        seq_t multiplier = ((seq_t)3 * cur) - 5;
        seq_t q_nm1 = multiplier * q_nm2;

        /* a_cur = 3*cur*a_nm1 + q_nm1 */
        seq_t term = ((seq_t)3 * cur) * a_nm1 + q_nm1;

        /* shift state for next iteration */
        a_nm2 = a_nm1;
        a_nm1 = term;
        q_nm2 = q_nm1;
    }
    return a_nm1;           /* a(n) */
}
