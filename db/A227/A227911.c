/*
 * A227911  Number of remainders occurring only once when dividing n by
 *          1,2,...n.
 * https://oeis.org/A227911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of remainders occurring exactly once when dividing
 *            n by 1,2,...,n
 * Range:     u8 n <= 152, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* not part of the offset, but safe */
    /* allocate frequency table */
    /* we need at most one entry per remainder 0..n-1 */
    uint16_t *freq = (uint16_t*)malloc((size_t)n * sizeof *freq);
    if (!freq) return (seq_t)0;             /* allocation failure → 0 as safe default */

    /* initialise to zero */
    for (size_t i = 0; i < (size_t)n; ++i) freq[i] = 0;

    /* count remainders */
    seq_t n_seq = (seq_t)n;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t rem = n_seq % (seq_t)k;        /* 0 <= rem < k <= n-1 */
        /* increment the count for this remainder */
        ++freq[(size_t)rem];
    }

    /* count those that appear exactly once */
    seq_t result = 0;
    for (size_t r = 0; r < (size_t)n; ++r) {
        if (freq[r] == 1) ++result;
    }

    free(freq);
    return result;
}
