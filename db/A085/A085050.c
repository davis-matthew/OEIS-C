/*
 * A085050  Smallest multiple of n sandwiched between two squarefree numbers.
 * https://oeis.org/A085050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 8362, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Description: smallest multiple of n such that both m-1 and m+1 are squarefree */
#include <stdbool.h>
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    /* k starts at 1; for n = 1 the first candidate m = 1 is skipped
       because 0 is not considered squarefree by the reference code */
    idx_t k = 1;
    for (;; ++k) {
        seq_t m = (seq_t)n * k;
        /* skip the trivial m = 1, which would have m-1 = 0 */
        if (m == 1)
            continue;
        if (is_squarefree(m - 1) && is_squarefree(m + 1))
            return m;
    }
}
