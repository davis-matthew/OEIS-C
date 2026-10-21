/*
 * A065160  Reduced binary string self-substitutions: a(n) is obtained by
 *          substituting n for each 1-bit in the binary expansion of n, then
 *          dividing by n.
 * https://oeis.org/A065160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Reduced binary string self-substitutions:
 * For a given n>=1, let base = binary representation of n, length k.
 * For each bit b of base from most significant to least:
 *     If b==1: append the binary string of n (k bits).
 *     If b==0: append a single 0 bit.
 * The resulting binary string is interpreted as an integer V.
 * The sequence term is a(n) = V / n.
 * This implementation builds V iteratively.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)  /* by convention */
        return 0;

    /* Determine the bit length k of n */
    idx_t t = n;
    int k = 0;
    while (t) {
        k++;
        t >>= 1;
    }

    seq_t V = 0;
    /* Process bits from most significant to least */
    for (int i = k - 1; i >= 0; --i) {
        int bit = (n >> i) & 1;
        if (bit)
            V <<= k, V |= (seq_t)n;   /* append binary of n */
        else
            V <<= 1;                  /* append a single 0 bit */
    }

    return V / (seq_t)n;
}
