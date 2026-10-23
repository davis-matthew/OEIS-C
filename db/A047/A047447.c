/*
 * A047447  Numbers that are congruent to {2, 3, 5, 6} mod 8.
 * https://oeis.org/A047447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n-1)/4) + {2,3,5,6}[(n-1)%4]
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k = (n - 1) / 4;          /* block number */
    idx_t r = (n - 1) % 4;          /* position inside the block */
    seq_t base = (seq_t)8 * k;      /* start of this block */

    switch (r)
    {
        case 0: return base + (seq_t)2;
        case 1: return base + (seq_t)3;
        case 2: return base + (seq_t)5;
        case 3: return base + (seq_t)6;
        default: return base;       /* unreachable */
    }
}
