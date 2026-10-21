/*
 * A052992  Expansion of 1/((1 - x)*(1 - 2*x)*(1 + 2*x)).
 * https://oeis.org/A052992
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∑_{j=0..⌊n/2⌋} 4^j = (4^(1+⌊n/2⌋)-1)/3
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* floor(n/2) */
    idx_t half = n >> 1;
    /* number of terms to add: 1 + floor(n/2) */
    idx_t k = half + 1;

    seq_t sum  = 0;
    seq_t term = 1;          /* 4^0 */

    for (idx_t i = 0; i < k; ++i) {
        sum += term;
        term *= (seq_t)4;   /* next power of 4 */
    }

    return sum;
}
