/*
 * A042515  Denominators of continued fraction convergents to sqrt(786).
 * https://oeis.org/A042515
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(0)=1; a(1)=28; for n>=2, a(n)= (n%2==0?56:28)*a(n-1)+a(n-2);
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 28;

    seq_t prev2 = 1;  /* a(0) */
    seq_t prev1 = 28; /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t coeff = (i & 1) ? 28 : 56;
        seq_t cur = coeff * prev1 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
