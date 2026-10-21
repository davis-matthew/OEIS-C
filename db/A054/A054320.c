/*
 * A054320  Expansion of g.f.: (1 + x)/(1 - 10*x + x^2).
 * https://oeis.org/A054320
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A054320:  a(0)=1, a(1)=11, a(n)=10*a(n-1)-a(n-2) for n≥2    */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)11;

    seq_t prev2 = (seq_t)1;   /* a(n-2)   */
    seq_t prev1 = (seq_t)11;  /* a(n-1)   */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur = prev1 * (seq_t)10 - prev2;  /* recurrence   */
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
