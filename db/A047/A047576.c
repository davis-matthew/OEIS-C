/*
 * A047576  Numbers that are congruent to {1, 5, 6, 7} mod 8.
 * https://oeis.org/A047576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n-1)/4) + {1,5,6,7}[ (n-1) mod 4 ]
 * Range:     u8 n <= 128, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 1‑based according to OEIS offset 1 */
    idx_t block = (n - 1) / 4;   /* integer division */
    idx_t rem   = (n - 1) % 4;   /* remainder 0..3 */

    seq_t val = (seq_t)(8 * block);   /* base value */

    switch (rem) {
        case 0: val += 1; break;
        case 1: val += 5; break;
        case 2: val += 6; break;
        case 3: val += 7; break;
    }
    return val;
}
