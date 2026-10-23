/*
 * A133633  Duplicate of A038509.
 * https://oeis.org/A133633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   not stated
 * Range:     i8 n <= 13, i16 n <= 17, i32 n <= 17, i64 n <= 17, i128 n <= 17,
 *            big any n
 * Helpers:   isCompositeIdx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: composite numbers that are coprime to 6 (i.e., >1, not divisible by 2 or 3, and composite). */
#include <stdbool.h>
#include "../utils/isCompositeIdx.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined for n = 0, but return 0 */
    idx_t count = 0;
    idx_t i = 25;                      /* first composite coprime to 6 */
    for (;; ++i) {
        if (((i & 1ULL) != 0ULL) && (i % 3ULL != 0ULL) && isCompositeIdx(i)) {
            ++count;
            if (count == n) return (seq_t)i;
        }
    }
}
