/*
 * A115012  Sum_{i=1..n, gcd(5,i)=1} i.
 * https://oeis.org/A115012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{i=1..n, gcd(5,i)=1} i = n*(n+1)/2 - 5 * floor(n/5) *
 *            (floor(n/5)+1)/2
 * Range:     u8 n <= 25, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;
    seq_t total = ns * (ns + 1) / 2;

    idx_t m = n / 5;                 /* floor(n/5) */
    seq_t ms = (seq_t)m;
    seq_t five_sum = (seq_t)5 * ms * (ms + 1) / 2;

    return total - five_sum;
}
