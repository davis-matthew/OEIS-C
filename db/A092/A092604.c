/*
 * A092604  Complement of A013939.
 * https://oeis.org/A092604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th positive integer not in A013939, where A013939(k) =
 *            Sum_{j=1..k} omega(j); found by walking the partial sums and
 *            counting the skipped values
 * Range:     u8 n <= 113, u16 n <= 14301, u32 n <= 14301, u64 n <= 14301,
 *            u128 n <= 14301, big any n
 * Validated: standard, pass, 78 terms through n = 9904 (2026-11-07)
 * Helpers:   omega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/omega.h"

seq_t a(idx_t n)
{
    idx_t rem = n;
    seq_t prev = 0; /* A013939(1) = 0 */
    for (uint64_t k = 2;; ++k) {
        idx_t w = (idx_t)omega(k);   /* >= 1 for k >= 2 */
        idx_t gap = w - 1;           /* skipped values: prev+1 .. prev+w-1 */
        if (gap >= rem) {
            return prev + (seq_t)rem;
        }
        rem -= gap;
        prev += (seq_t)w;
    }
}
