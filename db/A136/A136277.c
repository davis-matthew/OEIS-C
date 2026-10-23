/*
 * A136277  From the binary representation of n: binomial(number of ones,
 *          number of blocks of contiguous ones).
 * https://oeis.org/A136277
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(number_of_ones(n), number_of_blocks_of_ones(n))
 * Range:     u8 n <= 7802, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A136277 */

/* use the provided binomial utility */
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* count set bits */
    idx_t pop = 0;
    idx_t tmp  = n;
    while (tmp) {
        pop++;
        tmp &= tmp - 1;        /* clear lowest set bit */
    }

    /* count blocks of contiguous 1s */
    idx_t blocks = 0;
    tmp = n;
    while (tmp) {
        /* skip zeros */
        while ((tmp & 1ULL) == 0ULL && tmp) {
            tmp >>= 1;
        }
        if (!tmp) break;
        /* start of a block */
        blocks++;
        /* skip ones in this block */
        while ((tmp & 1ULL) == 1ULL && tmp) {
            tmp >>= 1;
        }
    }

    /* for n == 0 we get pop==0,blocks==0, binomial(0,0)=1 */
    return binomial(pop, blocks);
}
