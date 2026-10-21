/*
 * A210101  Number of (n+1) X 3 0..2 arrays with every 2 X 2 subblock having
 *          one or three distinct values, and new values 0..2 introduced in
 *          row major order.
 * https://oeis.org/A210101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n‑1) - 3*a(n‑2) - 4*a(n‑3) (n ≥ 4) with a(1) = 30,
 *            a(2) = 225, a(3) = 1690
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* external linkage, no side‑effects, works for both plain‑unsigned
   integral seq_t and arbitrary‑precision seq_t classes */
seq_t a(idx_t n)
{
    /* Handle indices below the offset trivially */
    if (n < 1) return (seq_t)0;

    /* base cases */
    if (n == 1) return (seq_t)30;
    if (n == 2) return (seq_t)225;
    if (n == 3) return (seq_t)1690;

    /* iterate using only the three previous values */
    seq_t a1 = (seq_t)30;      /* a(n‑3)   */
    seq_t a2 = (seq_t)225;     /* a(n‑2)   */
    seq_t a3 = (seq_t)1690;    /* a(n‑1)   */
    seq_t ai;

    for (idx_t i = 4; i <= n; ++i) {
        ai = (seq_t)8 * a3 - (seq_t)3 * a2 - (seq_t)4 * a1;
        a1 = a2;
        a2 = a3;
        a3 = ai;
    }
    return a3;
}
