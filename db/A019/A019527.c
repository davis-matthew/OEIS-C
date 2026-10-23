/*
 * A019527  Poincaré series [or Poincare series] for depths of roots in a
 *          certain root system.
 * https://oeis.org/A019527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-3) - 2 (n > 4) Initial values: a(1)=4,
 *            a(2)=4, a(3)=5, a(4)=6 (Recurrence derives from a(n)-2 =
 *            (a(n-1)-2)+(a(n-3)-2) )
 * Range:     u8 n <= 14, u16 n <= 29, u32 n <= 58, u64 n <= 116,
 *            u128 n <= 232, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly */
    if (n == 1) return 4;
    if (n == 2) return 4;
    if (n == 3) return 5;
    if (n == 4) return 6;

    /* Compute x(n) = a(n) - 2 which satisfies
       x(n) = x(n-1) + x(n-3)  for n >= 5
       with x(1)=2, x(2)=2, x(3)=3, x(4)=4   */
    seq_t prev3 = 2;   /* x(2)  */
    seq_t prev2 = 3;   /* x(3)  */
    seq_t prev1 = 4;   /* x(4)  */
    seq_t cur;

    for (idx_t i = 5; i <= n; ++i) {
        cur = prev1 + prev3;    /* x(i) = x(i-1) + x(i-3) */
        prev3 = prev2;          /* shift window: x(i-3) <- x(i-2) */
        prev2 = prev1;          /*                 x(i-2) <- x(i-1) */
        prev1 = cur;            /*                 x(i-1) <- x(i)   */
    }

    return prev1 + 2;  /* a(n) = x(n) + 2 */
}
