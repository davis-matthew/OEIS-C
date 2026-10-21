/*
 * A092076  Expansion of (1+4*x^3+x^6)/((1-x)*(1-x^3)^2).
 * https://oeis.org/A092076
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A003215(floor(n/3)) Equivalent: a(n) = 1 +
 *            3*floor(n/3)*(floor(n/3)+1)
 * Range:     u8 n <= 26, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute k = floor(n/3) */
    idx_t k_idx = n / 3;
    seq_t k = (seq_t)k_idx;

    /* a(n) = 1 + 3*k*(k+1) */
    return (seq_t)1 + (seq_t)3 * k * (k + 1);
}
