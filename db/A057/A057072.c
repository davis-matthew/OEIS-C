/*
 * A057072  a(n) = floor(10^10/n).
 * https://oeis.org/A057072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(10^10 / n)
 * Range:     u8 none, u16 none, u32 none, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 10^10 fits in 64 bits; cast to seq_t to match the type of n */
    return ((seq_t)(10000000000ULL)) / (seq_t)n;
}
