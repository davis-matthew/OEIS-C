/*
 * A051263  Expansion of 1/((1-x)*(1-x^3)^2*(1-x^5)).
 * https://oeis.org/A051263
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{c=0}^{floor(n/5)}
 *            ((floor((n-5c)/3)+1)*(floor((n-5c)/3)+2))/2
 * Range:     u8 n <= 35, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ans = 0;
    for (idx_t c = 0; c <= n / 5; ++c) {
        idx_t m = n - 5 * c;          /* remaining after taking c blocks of 5 */
        idx_t k = m / 3;              /* maximum number of 3-blocks */
        seq_t term = ((seq_t)(k + 1) * (seq_t)(k + 2)) / 2;
        ans += term;
    }
    return ans;
}
