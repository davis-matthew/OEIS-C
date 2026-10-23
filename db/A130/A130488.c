/*
 * A130488  a(n) = Sum_{k=0..n} (k mod 10) (Partial sums of A010879).
 * https://oeis.org/A130488
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 45*floor(n/10) + (n%10)*(n%10+1)/2
 * Range:     u8 n <= 57, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* number of complete blocks of 10 */
    seq_t q = (seq_t)(n / 10);
    /* remainder within current block */
    seq_t r = (seq_t)(n % 10);
    /* a(n) = 45*q + r*(r+1)/2 */
    return 45 * q + r * (r + 1) / 2;
}
