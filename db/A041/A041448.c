/*
 * A041448  Numerators of continued fraction convergents to sqrt(240).
 * https://oeis.org/A041448
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(0) = 15, a(1) = 31, for n >= 2: a(n) = ((n & 1) ? 2 : 30) *
 *            a(n-1) + a(n-2)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first two terms directly. */
    if (n == 0) return (seq_t)15;
    if (n == 1) return (seq_t)31;

    seq_t prev2 = (seq_t)15;   /* a(0) */
    seq_t prev1 = (seq_t)31;   /* a(1) */
    seq_t cur   = 0;           /* placeholder for a(n) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t coeff = (i & 1) ? (seq_t)2 : (seq_t)30;
        cur = coeff * prev1 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
