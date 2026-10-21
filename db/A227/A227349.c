/*
 * A227349  Product of lengths of runs of 1-bits in binary representation of
 *          n.
 * https://oeis.org/A227349
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = product of lengths of runs of 1-bits in binary
 *            representation of n
 * Range:     u8 n <= 8192, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t product = 1;
    bool in_run = false;
    idx_t len = 0;

    while (n) {
        if (n & 1ULL) {               /* current bit is 1 */
            if (!in_run) {            /* start a new run */
                in_run = true;
                len = 1;
            } else {                  /* extend current run */
                ++len;
            }
        } else {                      /* current bit is 0 */
            if (in_run) {            /* end of a run */
                product *= (seq_t)len;
                in_run = false;
            }
        }
        n >>= 1;                      /* move to next bit */
    }

    if (in_run) {                    /* finalize last run if string ends with 1 */
        product *= (seq_t)len;
    }

    return product;
}
