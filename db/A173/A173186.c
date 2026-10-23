/*
 * A173186  Numbers k such that k^2-1 is not squarefree.
 * https://oeis.org/A173186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 210, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the nth integer k such that k^2-1 is not squarefree */
#include <stdbool.h>
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k = 1;
    for (;;) {
        if (k & 1ull) {                     /* odd k → k^2‑1 always divisible by 4 */
            if (++count == n)
                return (seq_t)k;
        } else {
            seq_t kk = (seq_t)k;
            seq_t x  = kk * kk - 1;         /* compute k^2-1 */
            if (!is_squarefree(x)) {        /* if not squarefree, count it */
                if (++count == n)
                    return kk;
            }
        }
        ++k;
    }
}
