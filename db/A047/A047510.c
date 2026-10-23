/*
 * A047510  Numbers that are congruent to {2, 4, 6, 7} mod 8.
 * https://oeis.org/A047510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8 * ((n-1) / 4) + {2,4,6,7}[(n-1) % 4]
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is guaranteed to be >= 1 */
    idx_t idx = n - 1;              /* zero‑based index */
    idx_t m   = idx >> 2;           /* (n-1)/4 as integer division by 4 */
    unsigned r = (unsigned)(idx & 3);/* (n-1) mod 4, value 0..3 */

    static const seq_t map[4] = {
        (seq_t)2,
        (seq_t)4,
        (seq_t)6,
        (seq_t)7
    };

    return (seq_t)8 * (seq_t)m + map[r];
}
