/*
 * A039079  Numbers whose base-9 representation has the same number of 0's and
 *          3's.
 * https://oeis.org/A039079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th positive integer whose base-9 representation has
 *            an equal number of 0's and 3's
 * Range:     u8 n <= 167, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   same_zero_three_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/same_zero_three_uint64.h"

/* OEIS sequence A039079 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, just for safety */
    uint64_t found = 0;
    uint64_t cand   = 1;           /* start from 1 */
    while (1) {
        if (same_zero_three_uint64(cand)) {
            if (++found == n)
                return (seq_t)cand;
        }
        cand++;                      /* next candidate */
    }
}
