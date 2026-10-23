/*
 * A101674  Fixed point of morphism 0 -> 01, 1 -> 20, 2 -> 21.
 * https://oeis.org/A101674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Fixed point of morphism 0→01, 1→20, 2→21 */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Mapping table for the morphism: child[symbol][next_bit] */
    static const int child[3][2] = {
        {0, 1}, /* φ(0) = 0 1 */
        {2, 0}, /* φ(1) = 2 0 */
        {2, 1}  /* φ(2) = 2 1 */
    };

    int symbol = 0;          /* start symbol 0 */
    int msb = 0;             /* position of the most significant bit of n */
    idx_t t = n;
    while (t >>= 1) ++msb;   /* find floor(log2(n)); for n=0 msb stays 0 */

    for (int i = msb; i >= 0; --i) {
        int bit = (n >> i) & 1;          /* next bit of the index */
        symbol = child[symbol][bit];     /* move to the appropriate child */
    }

    return (seq_t)symbol;
}
