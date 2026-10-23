/*
 * A044193  Numbers n such that string 0,6 occurs in the base 8 representation
 *          of n but not of n-1.
 * https://oeis.org/A044193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   has_06
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044193: nth integer whose octal representation contains the
 * substring "06" but the predecessor does not. The function searches
 * sequentially, avoiding a hard-coded table. */
#include <stdbool.h>
#include "../utils/has_06.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* not part of the sequence */
    idx_t count = 0;

    for (idx_t m = 1; ; ++m) {
        seq_t cur  = (seq_t)m;
        if (has_06(cur)) {
            seq_t prev = (seq_t)(m - 1);
            if (!has_06(prev)) {
                ++count;
                if (count == n) return cur;
            }
        }
    }
}
