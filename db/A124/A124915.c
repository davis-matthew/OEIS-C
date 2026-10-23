/*
 * A124915  a(n) = least integer k>=0 such that n=Floor[(2^j)/(3^k)] for some
 *          integer j>=0.
 * https://oeis.org/A124915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least integer k>=0 such that n = Floor[(2^j)/(3^k)] for
 *            some integer j
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return bit length of a 128‑bit positive integer.
   bitlen(0) == 0, bitlen(x) == floor(log2(x))+1 for x>0. */
static unsigned bitlen128(unsigned __int128 v)
{
    unsigned len = 0;
    while (v)
    {
        v >>= 1;
        ++len;
    }
    return len;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not used, but keeps behaviour defined */

    unsigned __int128 pow3 = 1;     /* holds 3^k */
    for (idx_t k = 0; ; ++k)
    {
        unsigned __int128 L   = (unsigned __int128)n * pow3;             /* n*3^k  */
        unsigned __int128 high = ((unsigned __int128)n + 1) * pow3 - 1;   /* (n+1)*3^k-1 */

        /* Compute ceil(log2(L)) = bitlen(L-1)   (for L>0) */
        unsigned j_low  = bitlen128(L - 1);
        /* Compute floor(log2(high)) = bitlen(high)-1   (high>0 because n>=1,k>=0) */
        unsigned j_high = bitlen128(high) - 1;

        if (j_low <= j_high)
            return (seq_t)k;

        /* Prepare next power of three */
        pow3 *= 3u;
    }
}
