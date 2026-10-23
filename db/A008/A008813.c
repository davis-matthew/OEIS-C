/*
 * A008813  Expansion of (1+x^6)/((1-x)^2*(1-x^6)).
 * https://oeis.org/A008813
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n-4)*floor((n+1)/6) - 6*floor((n+1)/6)^2 + n + 1
 * Range:     u8 n <= 38, u16 n <= 626, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent rearrangement used below avoids negative intermediate values. */

seq_t a(idx_t n)
{
    seq_t k = ((seq_t)n + 1) / 6;                   /* k = floor((n+1)/6) */
    seq_t result = (seq_t)n * ((seq_t)2 * k + 1);   /* n * (2k + 1) */
    result -= 4 * k;
    result -= 6 * k * k;
    result += 1;
    return result;
}
