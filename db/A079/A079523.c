/*
 * A079523  Utterly odd numbers: numbers whose binary representation ends in
 *          an odd number of ones.
 * https://oeis.org/A079523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th integer (increasing order) whose binary
 *            representation * ends with an odd number of consecutive 1s
 *            (a.k.a. odd trailing‑ones)
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Count how many qualifying numbers we have found so far. */
    idx_t found = 0;

    /* Scan all positive integers until the n‑th qualifying one is reached. */
    for (idx_t x = 1; ; ++x) {
        /* Count trailing ones in x. */
        idx_t tmp = x, twos = 0;
        while (tmp & 1) {
            ++twos;
            tmp >>= 1;
        }

        /* If the count of trailing ones is odd, it belongs to the sequence. */
        if (twos & 1) {
            ++found;
            if (found == n)
                return (seq_t)x;
        }
    }
}
