/*
 * A098498  Number of squares on infinite half chessboard at <=n knight moves
 *          from a fixed point on the edge.
 * https://oeis.org/A098498
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*n^2 - n + 2 (for n > 3) Initial values: a(0)=1, a(1)=5,
 *            a(2)=23, a(3)=60
 * Range:     u8 n <= 6, u16 n <= 96, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the four small cases explicitly */
    if (n == 0) return 1;
    if (n == 1) return 5;
    if (n == 2) return 23;
    if (n == 3) return 60;

    /* For n >= 4 use the closed‑form expression.
       All operations are performed in seq_t to avoid overflow
       of intermediate values when seq_t is wider than idx_t. */
    seq_t nn = (seq_t)n;          /* copy index to seq_t */
    seq_t res = nn * nn;          /* n^2  */
    res = res * (seq_t)7;         /* 7*n^2 */
    res -= nn;                    /* - n   */
    res += (seq_t)2;              /* + 2   */
    return res;
}
