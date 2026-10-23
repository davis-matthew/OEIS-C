/*
 * A124737  Table where row n has k C(n,k) times.
 * https://oeis.org/A124737
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   a(n) is the value at index n in the concatenated rows of the
 *            array defined by: row r contains each integer k repeated C(r,k)
 *            times, for k = 0..r. The sequence is obtained by listing the
 *            rows in order (row 0 first)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   binom_n_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/binom_n_k.h"

seq_t a(idx_t n)
{
    /* Determine the row r that contains position n.
       The cumulative number of elements up to row r is 2^(r+1)-1.
       Hence r = floor(log2(n+1)).  */
    idx_t r = 63 - __builtin_clzll(n + 1);          /* floor(log2(n+1)) */

    /* Number of elements before row r. */
    idx_t before = ((idx_t)1 << r) - 1;             /* 2^r - 1 */

    /* Offset of position n within row r. */
    idx_t offset = n - before;

    /* Find the k such that the offset falls within the block of k's
       repeated C(r,k) times. */
    idx_t cumulative = 0;
    for (idx_t k = 0; k <= r; ++k) {
        seq_t bin = binom_n_k(r, k);               /* C(r,k) */
        cumulative += (idx_t)bin;                  /* cumulative sum is <= 2^r <= 2^63-1 */
        if (offset < cumulative)
            return (seq_t)k;
    }

    /* Should never reach here because all positions are covered. */
    return 0;
}
