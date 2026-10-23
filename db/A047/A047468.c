/*
 * A047468  Numbers that are congruent to {1, 2} mod 8.
 * https://oeis.org/A047468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + 8*floor((n-1)/2) + ((n-1) mod 2)
 * Range:     u8 n <= 64, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 1-based index */
    seq_t n_seq     = (seq_t)n;
    seq_t n_minus1  = n_seq - (seq_t)1;
    seq_t half_floor= n_minus1 / (seq_t)2;
    seq_t parity    = n_minus1 % (seq_t)2;
    return ((seq_t)1) + ((seq_t)8) * half_floor + parity;
}
