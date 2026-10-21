/*
 * A075897  1 if binary weight of n is 1 or 2, otherwise 0.
 * https://oeis.org/A075897
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if binary weight (popcount) of n is 1 or 2, 0
 *            otherwise; a(0)=0
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t x = n;
    unsigned int popcnt = 0;
    while (x) {
        x &= x - 1;      /* clear lowest set bit */
        popcnt++;
    }
    return (popcnt == 1 || popcnt == 2) ? (seq_t)1 : (seq_t)0;
}
