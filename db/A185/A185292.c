/*
 * A185292  Expansion of (x*(1+x)/(1-x^3))^4.
 * https://oeis.org/A185292
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ∑_{k=0}^4 binom(4,k) * binom( (n−4−k)/3 + 3 , 3 ) (terms
 *            taken only if n ≥ 4+k and (n−4−k) mod 3 = 0)
 * Range:     u8 n <= 18, u16 n <= 121, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const idx_t coeff[5] = {1, 4, 6, 4, 1};   /* binom(4,k) */
    seq_t result = 0;

    for (idx_t k = 0; k < 5; ++k) {
        idx_t base = 4 + k;
        if (n < base) continue;                /* no contribution */
        idx_t diff = n - base;
        if (diff % 3 != 0) continue;           /* not a multiple of 3 */
        idx_t m = diff / 3;                    /* m ≥ 0 */
        /* binom(3+m,3) = (m+1)(m+2)(m+3)/6 */
        seq_t prod = (seq_t)(m + 1) * (m + 2) * (m + 3);
        seq_t binom = prod / 6;
        result += binom * (seq_t)coeff[k];
    }

    return result;
}
