/*
 * A154325  Triangle with interior all 2's and borders 1.
 * https://oeis.org/A154325
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = 1 for n = 0,1,2,3 and for all n where the position n lies
 *            in a run of length 2 of “1”s that follows the recursive block
 *            construction: block k (k≥1): a run of k “2”s followed by a run
 *            of 2 “1”s. a(n) = 2 otherwise
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Upper limit of n is about 10000, but the algorithm works for any
   n >= 0 in O(√n) time.
*/
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* For the first four terms the value is 1. */
    if (n < 4) {
        return (seq_t)1;
    }

    /* Position after the initial block of four 1’s. */
    idx_t offset = n - 4;

    /* Find the block index m such that
         S_{m-1} <= offset < S_m
       where S_m = m(m+1)/2 + 2m = (m^2 + 5m)/2  (sum up to block m). */
    idx_t m = 1;
    idx_t cumulative = 0;               /* S_{m-1} */
    idx_t block_len;                    /* length of block m = m + 2 */

    while (1) {
        block_len = m + 2;                       /* two-run of length m + one-run of length 2 */
        if (cumulative + block_len > offset) {
            break;                               /* offset lies in this block */
        }
        cumulative += block_len;
        m++;
    }

    /* Position within the block. */
    idx_t pos = offset - cumulative;

    /* Within block m: first m positions are "2"s, next 2 positions are "1"s. */
    if (pos < (idx_t)m) {
        return (seq_t)2;   /* in the run of "2"s */
    } else {
        return (seq_t)1;   /* in the run of "1"s */
    }
}
