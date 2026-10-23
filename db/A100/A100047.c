/*
 * A100047  A Chebyshev transform of the Fibonacci numbers.
 * https://oeis.org/A100047
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, mult
 * Formula:   a(n) = a(n-1) - a(n-2) + a(n-3) - a(n-4) Initial values: a(0)=0,
 *            a(1)=1, a(2)=1, a(3)=-1
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base values for n = 0,1,2,3 */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)1;
    /* n == 3 */
    if (n == 3) return (seq_t)-1;

    /* Use a ring buffer of the last four terms: a[i-4], a[i-3], a[i-2], a[i-1] */
    seq_t a0 = (seq_t)0;   /* a(i-4)  */
    seq_t a1 = (seq_t)1;   /* a(i-3)  */
    seq_t a2 = (seq_t)1;   /* a(i-2)  */
    seq_t a3 = (seq_t)-1;  /* a(i-1)  */
    seq_t ai;              /* a(i)    */

    for (idx_t i = 4; i <= n; ++i) {
        ai = a3 - a2 + a1 - a0; /* a(i) = a(i-1) - a(i-2) + a(i-3) - a(i-4) */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = ai;
    }
    return a3;
}
