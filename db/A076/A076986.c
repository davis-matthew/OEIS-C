/*
 * A076986  Smallest squarefree number of the form n*k + 1.
 * https://oeis.org/A076986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search implementation:  a(n) = smallest integer >1 congruent to 1 (mod n) that is squarefree. */
#include <stdbool.h>
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    seq_t m;
    for (idx_t k = 1; ; ++k) {
        m = (seq_t)n * k + 1;
        if (is_squarefree(m))
            return m;
    }
}
