/*
 * A238738  Expansion of (1 + 2*x + 2*x^2)/(1 - x - 2*x^3 + 2*x^4 + x^6 -
 *          x^7).
 * https://oeis.org/A238738
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (floor(n/3)+1)*(4*n-7*floor(n/3)+2)/2
 * Range:     u8 n <= 28, u16 n <= 483, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k = n / 3;
    seq_t kseq = (seq_t)k;
    seq_t term1 = kseq + 1;
    seq_t term2 = 4 * (seq_t)n - 7 * kseq + 2;
    seq_t result = term1 * term2 / 2;
    return result;
}
