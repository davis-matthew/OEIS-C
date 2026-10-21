/*
 * A203228  (n-1)-st elementary symmetric function of (1!,...,(n-1)!).
 * https://oeis.org/A203228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1 Recurrence: for n >= 2 a(n) = n! * a(n-1) + D(n) where
 *            D(2) = 1 and D(n) = (n-1)! * D(n-1) (D(n) = ∏_{k=2}^{n-1} k!)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    seq_t a_prev = (seq_t)1;   /* a(1)                         */
    seq_t fact   = (seq_t)1;   /* (1)!  at start */
    seq_t d      = (seq_t)1;   /* D(2)  = product_{k=2}^{1} k! = 1 */

    for (idx_t i = 2; i <= n; ++i)
    {
        /* i!  = i * fact   (fact currently holds (i-1)!)  */
        seq_t fact_i = (seq_t)i * fact;

        /* a(i) = i! * a(i-1) + D(i) (= d)               */
        a_prev = fact_i * a_prev + d;

        /* Update D for the next iteration: D(i+1) = D(i) * i! */
        d      = d * fact_i;

        /* Update fact to i! for next loop iteration      */
        fact   = fact_i;
    }

    return a_prev;
}
