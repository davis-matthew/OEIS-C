/*
 * A223475  Least k such that the decimal representation of k*n has digits in
 *          nonincreasing order.
 * https://oeis.org/A223475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 362, u32 n <= 1088, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   nonincreasing
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Calculates the least k such that the decimal representation of k*n
   has digits in nonincreasing order.
   (OEIS A223475) */
#include <stdbool.h>
#include "../utils/nonincreasing.h"

seq_t a(idx_t n)
{
    if (n == 0)                /* defined only for n>=1, but make
                                 it safe for 0 */
        return 0;

    seq_t sn = (seq_t) n;       /* cast index to seq_t */
    for (idx_t k = 1;; ++k) {
        seq_t m = sn * (seq_t)k;   /* current product */
        if (nonincreasing(m))
            return (seq_t)k;       /* found minimal k */
    }
}
