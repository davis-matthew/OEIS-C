/*
 * A047452  Numbers that are congruent to {1, 6} mod 8.
 * https://oeis.org/A047452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n - 3 (n odd), 4*n - 2 (n even)
 * Range:     u8 n <= 64, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset starts at 1 */
    seq_t val = (seq_t)n * 4;
    if (n & 1)        /* odd n */
        val -= 3;
    else              /* even n */
        val -= 2;
    return val;
}
