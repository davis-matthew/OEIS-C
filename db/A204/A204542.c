/*
 * A204542  Numbers that are congruent to {1, 4, 11, 14} mod 15.
 * https://oeis.org/A204542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(15*n/4) - ((n + 1) mod 4)
 * Range:     u8 n <= 17, u16 n <= 4965, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* safety for indices below the offset */
    seq_t prod = (seq_t)15 * (seq_t)n;
    seq_t val  = prod / (seq_t)4;
    seq_t mod  = (seq_t)((n + 1) % (idx_t)4);
    return val - mod;
}
