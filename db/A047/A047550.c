/*
 * A047550  Numbers that are congruent to {5, 7} mod 8.
 * https://oeis.org/A047550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n - (-1)^n
 * Range:     u8 n <= 64, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = ((seq_t)n) * 4;
    if (n & 1ULL)
        res += 1;   /* n odd → (+1) */
    else
        res -= 1;   /* n even → (-1) */
    return res;
}
