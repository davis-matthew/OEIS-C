/*
 * A047276  Numbers that are congruent to {2, 6} mod 7.
 * https://oeis.org/A047276
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n + floor(n/2) - 1
 * Range:     u8 n <= 73, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the result type for safe multiplication */
    seq_t n_seq = (seq_t)n;
    /* floor(n/2) is just n/2 for unsigned integers */
    return n_seq * 3 + (seq_t)(n / 2) - (seq_t)1;
}
