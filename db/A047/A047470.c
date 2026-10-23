/*
 * A047470  Numbers that are congruent to {0, 3} mod 8.
 * https://oeis.org/A047470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n - (9 + (-1)^n)/2
 * Range:     u8 n <= 64, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 4 * (seq_t)n;
    if (n & 1ULL)
        val -= 4;
    else
        val -= 5;
    return val;
}
