/*
 * A228748  Pell numbers (A000129) minus Lucas numbers beginning at 2
 *          (A000032).
 * https://oeis.org/A228748
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Pell(n) - Lucas(n) Pell(0)=0, Pell(1)=1,
 *            Pell(n)=2*Pell(n-1)+Pell(n-2) Lucas(0)=2, Lucas(1)=1,
 *            Lucas(n)=Lucas(n-1)+Lucas(n-2)
 * Range:     i8 n <= 6, i16 n <= 12, i32 n <= 25, i64 n <= 50, i128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle small indices directly */
    if (n == 0) return (seq_t)-2;        /* 0 - 2 */
    if (n == 1) return (seq_t)0;         /* 1 - 1 */

    /* iterative computation */
    seq_t p_prev2 = (seq_t)0;          /* Pell(n-2)   */
    seq_t p_prev1 = (seq_t)1;          /* Pell(n-1)   */
    seq_t l_prev2 = (seq_t)2;          /* Lucas(n-2)  */
    seq_t l_prev1 = (seq_t)1;          /* Lucas(n-1)  */
    seq_t p, l;

    for (idx_t i = 2; i <= n; ++i) {
        p = (seq_t)2 * p_prev1 + p_prev2;  /* Pell recurrence */
        l = l_prev1 + l_prev2;             /* Lucas recurrence */
        p_prev2 = p_prev1;
        p_prev1 = p;
        l_prev2 = l_prev1;
        l_prev1 = l;
    }

    return p_prev1 - l_prev1;  /* Pell(n) - Lucas(n) */
}
