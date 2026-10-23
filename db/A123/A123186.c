/*
 * A123186  a(3*n-2) = a(3*n-1) = a(3*n) = b(n), where b(1) = 1, b(2) = -2,
 *          b(3) = 0, and b(n+1) = -(m+1)*b(n) - (m-1)*(m-2)*b(n-1) -
 *          (m-3)*(m-4)*(m-5)*b(n-2) for m = 3*n.
 * https://oeis.org/A123186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   a(3*n-2) = a(3*n-1) = a(3*n) = b(n) with b(1)=1, b(2)=-2,
 *            b(3)=0, and for n>=1 b(n+1)= -(3*n+1)*b(n) -
 *            (3*n-1)*(3*n-2)*b(n-1) - (3*n-3)*(3*n-4)*(3*n-5)*b(n-2)
 * Range:     i8 n <= 12, i16 n <= 21, i32 n <= 27, i64 n <= 35, i128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Determine which b(k) corresponds to index n */
    idx_t k = (n + 2) / 3;          /* ceil(n/3) */
    if (k == 1) return (seq_t)1;
    if (k == 2) return (seq_t)-2;
    if (k == 3) return (seq_t)0;

    /* Initialize first three terms */
    seq_t b_prev2 = (seq_t)1;   /* b(1) */
    seq_t b_prev1 = (seq_t)-2;  /* b(2) */
    seq_t b_prev  = (seq_t)0;   /* b(3) */

    for (idx_t i = 4; i <= k; ++i)
    {
        /* In the recurrence, b(i) depends on n = i-1 */
        idx_t n_cur = i - 1;
        seq_t m = (seq_t)(3 * (seq_t)n_cur);  /* m = 3*n_cur */

        seq_t term1 = -(m + (seq_t)1) * b_prev;
        seq_t term2 = -(m - (seq_t)1) * (m - (seq_t)2) * b_prev1;
        seq_t term3 = -(m - (seq_t)3) * (m - (seq_t)4) * (m - (seq_t)5) * b_prev2;
        seq_t next = term1 + term2 + term3;

        /* Shift for next iteration */
        b_prev2 = b_prev1;
        b_prev1 = b_prev;
        b_prev  = next;
    }

    return b_prev;   /* b(k) */
}
