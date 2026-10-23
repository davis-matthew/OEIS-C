/*
 * A160723  First differences of A160722.
 * https://oeis.org/A160723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*A001316(n-1) - 2 where A001316(k) = 2^{popcount(k)},
 *            i.e. 1 shifted left by the number of 1‑bits in k
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uint64_t type used by the build, if needed */

/* Count the number of 1 bits in an idx_t value (fast Hamming‑weight) */
static unsigned popcount_idx(idx_t v)
{
    unsigned c = 0;
    while (v)
    {
        v &= v - 1;  /* clear the lowest set bit */
        ++c;
    }
    return c;
}

seq_t a(idx_t n)
{
    if (n == 0)  /* safety for indices below the offset */
        return 0;

    idx_t m = n - 1;                     /* argument to A001316 */
    unsigned pc = popcount_idx(m);       /* popcount(m) */
    seq_t t = ((seq_t)1) << pc;         /* 2^popcount(m) */
    return 3 * t - (seq_t)2;            /* 3*A001316(m) - 2 */
}
