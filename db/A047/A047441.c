/*
 * A047441  Numbers that are congruent to {0, 2, 5, 6} mod 8.
 * https://oeis.org/A047441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n-1)/4) + {0,2,5,6}[ (n-1) mod 4 ] Sequence
 *            A047441: numbers congruent to 0,2,5,6 (mod 8) Offset: 1 (a(1)=0)
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)          // offset is 1; for completeness
        return (seq_t)0;

    /* integer part of block number */
    idx_t k = (n - 1) / 4;
    idx_t r = (n - 1) % 4;

    /* base value of the current block: multiples of 8 */
    seq_t base = (seq_t)8 * k;

    static const seq_t offs[4] = { (seq_t)0, (seq_t)2, (seq_t)5, (seq_t)6 };

    return base + offs[r];
}
