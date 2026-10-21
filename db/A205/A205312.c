/*
 * A205312  Number of (n+1) X 3 0..1 arrays with every 2 X 2 subblock having
 *          the same number of equal edges, and new values 0..1 introduced in
 *          row major order.
 * https://oeis.org/A205312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 7*a(n-2) + 2*a(n-3) (empirical) Initial
 *            values: a(1)=20, a(2)=84, a(3)=376
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 28, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)20;
    if (n == 2) return (seq_t)84;
    if (n == 3) return (seq_t)376;

    seq_t prev3 = (seq_t)20;  /* a(n-3)  */
    seq_t prev2 = (seq_t)84;  /* a(n-2)  */
    seq_t prev1 = (seq_t)376; /* a(n-1)  */

    for (idx_t i = 4; i <= n; ++i) {
        /* Compute cur = 6*prev1 + 2*prev3 - 7*prev2
           The subtraction is safe because cur > 0, so
           6*prev1 + 2*prev3 >= 7*prev2.           */
        seq_t cur = prev1 * (seq_t)6 + prev3 * (seq_t)2;
        cur -= prev2 * (seq_t)7;

        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;   /* prev1 holds a(n) after loop */
}
