/*
 * A130483  a(n) = Sum_{k=0..n} (k mod 5) (Partial sums of A010874).
 * https://oeis.org/A130483
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*floor(n/5) + (n mod 5)*((n mod 5)+1)/2
 * Range:     u8 n <= 127, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t seq_n = (seq_t)n;
    seq_t q = seq_n / 5;
    seq_t r = seq_n % 5;
    return q * 10 + r * (r + 1) / 2;
}
