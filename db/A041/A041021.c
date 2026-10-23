/*
 * A041021  Denominators of continued fraction convergents to sqrt(14).
 * https://oeis.org/A041021
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(0)=1, a(1)=1 * a(n)=coeff[(n-1)%4]*a(n-1)+a(n-2) for n≥2 *
 *            coeff = {1,2,1,6} (the periodic partial quotients of sqrt(14))
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 27, u64 n <= 53, u128 n <= 105,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first two terms explicitly. */
    if (n == 0U) return (seq_t)1;
    if (n == 1U) return (seq_t)1;

    /* Previous two denominators: a(n-2) and a(n-1). */
    seq_t prev2 = (seq_t)1;  /* a(0) */
    seq_t prev1 = (seq_t)1;  /* a(1) */
    seq_t cur    = (seq_t)0;

    /* Coefficients repeat every 4 steps after the first term. */
    static const unsigned coeff[4] = { 1, 2, 1, 6 };

    /* Compute a(n) iteratively. */
    for (idx_t i = 2U; i <= n; ++i) {
        unsigned c = coeff[(i - 1U) & 3U];   /* (i-1)%4 using bitwise AND */
        cur = (seq_t)c * prev1 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
