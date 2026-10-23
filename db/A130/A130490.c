/*
 * A130490  a(n) = Sum_{k=0..n} (k mod 12) (Partial sums of A010881).
 * https://oeis.org/A130490
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 66*floor(n/12) + (n%12)*((n%12)+1)/2
 * Range:     u8 n <= 46, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* m = n mod 12 as seq_t */
    seq_t m = (seq_t)(n % 12);

    /* triangular number of m: m*(m+1)/2 */
    seq_t t = m * (m + 1) / 2;

    /* add 66*floor(n/12) */
    seq_t res = 66 * (seq_t)(n / 12) + t;

    return res;
}
