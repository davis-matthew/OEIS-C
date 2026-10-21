/*
 * A061470  First (leftmost) digit - second digit + third digit - fourth digit
 *          .... = 1.
 * https://oeis.org/A061470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 3959, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alt_sum_is_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the n-th positive integer whose alternating sum of
 * decimal digits (leftmost minus next plus next ...) equals 1.
 * The implementation simply scans successive integers and returns the
 * n-th one that satisfies this condition.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/alt_sum_is_one.h"

/* Return the n-th term of the sequence. */
seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t cur = 1;

    while (1) {
        if (alt_sum_is_one(cur)) {
            ++count;
            if (count == n)
                return cur;
        }
        ++cur;
    }
}
