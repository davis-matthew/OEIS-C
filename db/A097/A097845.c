/*
 * A097845  Chebyshev polynomials S(n,171) + S(n-1,171) with Diophantine
 *          property.
 * https://oeis.org/A097845
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 171*a(n-1) - a(n-2), a(0) = 1, a(1) = 172
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)172;

    seq_t prev2 = (seq_t)1;     /* a(0) */
    seq_t prev1 = (seq_t)172;   /* a(1) */
    seq_t curr = prev1;         /* placeholder for a(n) */
    idx_t i;

    for (i = 2; i <= n; ++i) {
        curr = (seq_t)171 * prev1 - prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return curr;
}
