/*
 * A179952  Add 1 to all the divisors of n. a(n) = number of perfect squares
 *          in the set.
 * https://oeis.org/A179952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integers m >= 2 such that (m^2 - 1) divides n
 * Range:     u8 n <= 1703, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Equivalently, a(n) = # of divisors d of n with d+1 a perfect square) */

#include <stdint.h>  /* for uint64_t */

seq_t a(idx_t n)
{
    seq_t count = 0;
    /* iterate m = 2,3,... until m^2 - 1 > n */
    for (idx_t m = 2; ; ++m)
    {
        /* Use 128-bit arithmetic to avoid overflow when computing m*m */
        unsigned __int128 sq = (unsigned __int128)m * m;
        unsigned __int128 d  = sq - 1;              /* d = m^2 - 1 <= n */

        if (d > (unsigned __int128)n)               /* stop when d > n */
            break;

        seq_t d_seq = (seq_t)(uint64_t)d;           /* d fits in 64 bits */
        if (((seq_t)n) % d_seq == 0)                /* d divides n ? */
            ++count;
    }
    return count;
}
