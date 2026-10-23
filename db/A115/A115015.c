/*
 * A115015  Sum_{i=1..n} (gcd(7,i)=1) i.
 * https://oeis.org/A115015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n+1)/2 - 7*floor(n/7)*(floor(n/7)+1)/2
 * Range:     u8 n <= 23, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t total   = (seq_t)n * ((seq_t)n + 1) / 2;
    idx_t  m      = n / 7;                         /* floor division */
    seq_t multiples = (seq_t)7 * (seq_t)m * ((seq_t)m + 1) / 2;
    return total - multiples;
}
