/*
 * A130487  a(n) = Sum_{k=0..n} (k mod 9) (Partial sums of A010878).
 * https://oeis.org/A130487
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 36*floor(n/9) + (n mod 9)*(n mod 9 + 1)/2
 * Range:     u8 n <= 65, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t mod   = (seq_t)(n % 9);          /* n mod 9 */
    seq_t sum   = mod * (mod + 1) / 2;     /* triangular number of mod */
    seq_t floor = (seq_t)(n / 9);          /* floor(n/9) */
    return ((seq_t)36 * floor) + sum;     /* final result */
}
