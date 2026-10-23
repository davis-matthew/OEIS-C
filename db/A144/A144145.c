/*
 * A144145  A positive integer k is included if n contains the same number of
 *          0's as 1's when represented in binary and the number of 0's (or
 *          the number of 1's) divides k.
 * https://oeis.org/A144145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy, changed
 * Formula:   a(n) is the n‑th integer k such that the binary * representation
 *            of k contains the same number of 0’s as 1’s * and that number
 *            (the popcount) divides k. * The routine searches sequentially
 *            for the terms; no memoization * is used, which is sufficient up
 *            to the required limits
 * Range:     u8 n <= 14, u16 n <= 1142, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0)  /* OEIS offset is 1, so n==0 is undefined */
        return (seq_t)0;

    idx_t found = 0;          /* number of terms found so far   */
    idx_t k = 1;              /* candidate integer              */

    while (found < n) {
        /* binary length of k (k > 0) */
        int bitlen = 64 - __builtin_clzll(k);
        int ones   = __builtin_popcountll(k);
        int zeros  = bitlen - ones;

        if (ones == zeros && ones > 0 && (k % ones) == 0) {
            ++found;
            if (found == n)
                return (seq_t)k;
        }
        ++k;
    }
    /* unreachable */
    return (seq_t)0;
}
