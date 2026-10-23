/*
 * A114199  Row sums of a Pascal-Fibonacci triangle.
 * https://oeis.org/A114199
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*a(n-1) - 5*a(n-2) + 2*a(n-3) + a(n-4) for n ≥ 4 *
 *            Initial values: a(0)=1, a(1)=2, a(2)=4, a(3)=8
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 27, u64 n <= 54, u128 n <= 108,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth term of A114199 */
seq_t a(idx_t n)
{
    /* Initial terms for n = 0, 1, 2, 3 */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)8;

    /* t0, t1, t2, t3 correspond to a(n-4), a(n-3), a(n-2), a(n-1) */
    seq_t t0 = (seq_t)1;   /* a(0)   */
    seq_t t1 = (seq_t)2;   /* a(1)   */
    seq_t t2 = (seq_t)4;   /* a(2)   */
    seq_t t3 = (seq_t)8;   /* a(3)   */

    for (idx_t i = 4; i <= n; ++i) {
        /* Compute a(i) using the recurrence */
        seq_t new_val = t3 * (seq_t)4 + t1 * (seq_t)2 + t0 - t2 * (seq_t)5;
        /* Shift the window for the next iteration */
        t0 = t1;
        t1 = t2;
        t2 = t3;
        t3 = new_val;
    }
    return t3;
}
