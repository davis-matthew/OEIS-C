/*
 * A047410  Numbers that are congruent to {2, 4, 6} mod 8.
 * https://oeis.org/A047410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*floor((n-1)/3)+2*n
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Ensure n >= 1; behaviour for n < 1 is undefined as per requirement. */
    seq_t n_seq = (seq_t)n;
    seq_t t = (seq_t)(n - 1) / 3;
    return 2 * t + 2 * n_seq;
}
