/*
 * A047296  Numbers that are congruent to {3, 4, 6} mod 7.
 * https://oeis.org/A047296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*((n-1)/3) + {3,4,6}[(n-1)%3]
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset starts at 1 */
    idx_t q = (n - 1) / 3;                    /* block number */
    idx_t r = (n - 1) % 3;                    /* position within the block */

    seq_t base;
    switch (r) {
        case 0: base = 3; break;             /* 3 mod 7 */
        case 1: base = 4; break;             /* 4 mod 7 */
        default: base = 6; break;            /* 6 mod 7 */
    }

    return ((seq_t)q) * 7 + base;
}
