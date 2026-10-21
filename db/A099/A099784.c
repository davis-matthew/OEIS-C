/*
 * A099784  a(n) = Sum_{k=0..floor(n/3)} C(n-k,2*k) * 2^k * (-2)^(n-3*k).
 * https://oeis.org/A099784
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = -4*a(n-1) - 4*a(n-2) + 2*a(n-3), a(0)=1, a(1)=-2, a(2)=4
 * Range:     i8 n <= 6, i16 n <= 13, i32 n <= 25, i64 n <= 52, i128 n <= 102,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle initial cases directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-2;
    if (n == 2) return (seq_t)4;

    /* Iteratively compute using the recurrence */
    seq_t a0 = (seq_t)1;     /* a(n-3)   */
    seq_t a1 = (seq_t)-2;    /* a(n-2)   */
    seq_t a2 = (seq_t)4;     /* a(n-1)   */
    seq_t ai;
    for (idx_t i = 3; i <= n; ++i) {
        ai = (- (seq_t)4) * a2      /* -4*a(n-1) */
            + (- (seq_t)4) * a1      /* -4*a(n-2) */
            +  (seq_t)2  * a0;       /* +2*a(n-3) */

        /* Shift the window for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = ai;
    }

    return a2; /* a(n) is now stored in a2 */
}
