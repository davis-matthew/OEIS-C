/*
 * A102393  A wicked evil sequence.
 * https://oeis.org/A102393
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n+1) if popcount(n) is even, otherwise 0 (equivalent to
 *            a(n) = (n+1)*(1+(-1)^popcount(n))/2
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute parity of the number of 1‑bits of n */
    idx_t x = n;
    int parity = 0;
    while (x)
    {
        parity ^= 1;          /* toggle bit parity */
        x &= x - 1;           /* drop the lowest set bit */
    }
    /* parity==0 -> even popcount, return n+1; else return 0 */
    return (parity ? (seq_t)0 : ((seq_t)n + (seq_t)1));
}
