/*
 * A156866  a(n) = 729000*n - 116820.
 * https://oeis.org/A156866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 729000*n - 116820
 * Range:     u8 none, u16 none, u32 n <= 5891, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return ((seq_t)729000 * (seq_t)n) - (seq_t)116820;
}
