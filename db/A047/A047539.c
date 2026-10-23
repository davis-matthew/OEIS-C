/*
 * A047539  Numbers that are congruent to {2, 4, 7} mod 8.
 * https://oeis.org/A047539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((8*n-2)/3)
 * Range:     u8 n <= 31, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* Offset is 1; values for n<1 are undefined */
    seq_t n_seq = (seq_t)n;
    seq_t prod   = (seq_t)8 * n_seq;
    seq_t val    = (prod - (seq_t)2) / (seq_t)3;
    return val;
}
