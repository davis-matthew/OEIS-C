/*
 * A094821  Number of (s(0), s(1), ..., s(2n)) such that 0 < s(i) < 8 and
 *          |s(i) - s(i-1)| = 1 for i = 1,2,...,2n, s(0) = 3, s(2n) = 5.
 * https://oeis.org/A094821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 10*a(n-2) + 4*a(n-3), with a(1) = 1, a(2) = 4,
 *            a(3) = 15
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 36, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n < 1 */

    /* First three terms, index 1..3 */
    static const seq_t init[3] = { (seq_t)1, (seq_t)4, (seq_t)15 };

    /* For n = 1,2,3 return the pre‑computed value. */
    if (n <= 3) return init[n - 1];

    /* a1 = a(n-3), a2 = a(n-2), a3 = a(n-1) */
    seq_t a1 = init[0];   /* a(1)   */
    seq_t a2 = init[1];   /* a(2)   */
    seq_t a3 = init[2];   /* a(3)   */

    for (idx_t i = 4; i <= n; ++i)
    {
        /* a(i) = 6*a(i-1) - 10*a(i-2) + 4*a(i-3)   */
        /* Compute in two steps to avoid unsigned underflow. */
        seq_t temp = a3 * (seq_t)6 + a1 * (seq_t)4; /* 6*a3 + 4*a(i-3) */
        temp -= a2 * (seq_t)10;                    /* -10*a2 */

        /* Advance the sliding window */
        a1 = a2;
        a2 = a3;
        a3 = temp;
    }

    return a3;   /* a3 holds a(n) after the loop */
}
