/*
 * A042213  Denominators of continued fraction convergents to sqrt(632).
 * https://oeis.org/A042213
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(0) = 1 a(n) = c(n) * a(n-1) + a(n-2) (n ≥ 1) where the
 *            coefficient sequence (c(n)) is periodic with period 4: 7, 6, 7,
 *            50, 7, 6, 7, 50, … This recurrence gives the denominators of the
 *            convergents to √632
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;                  /* a(0) = 1 */

    static const idx_t period[4] = {7, 6, 7, 50};

    seq_t prev2 = 0;   /* a(-1) in the recurrence */
    seq_t prev1 = 1;   /* a(0)   */
    seq_t curr;

    for (idx_t i = 1; i <= n; ++i) {
        idx_t coeff = period[(i - 1) % 4];        /* cycle 7,6,7,50 */
        curr = (seq_t)coeff * prev1 + prev2;      /* a(i) = coeff*a(i-1)+a(i-2) */
        prev2 = prev1;
        prev1 = curr;
    }
    return curr;   /* now curr == a(n) */
}
