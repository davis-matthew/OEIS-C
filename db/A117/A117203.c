/*
 * A117203  Odd squarefree positive integers k such that (k-1)/2 is also
 *          squarefree.
 * https://oeis.org/A117203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*A117204(n) + 1
 * Range:     u8 n <= 65, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   numbers k that are odd, squarefree and (k-1)/2 is also squarefree */

#include <stdbool.h>
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t candidate = 1;   /* start before the first odd integer > 1 */

    for (;;) {
        candidate += 2;                                 /* next odd integer */
        if (is_squarefree(candidate) &&
            is_squarefree((candidate - 1) / 2)) {
            ++count;
            if (count == n)
                return candidate;
        }
    }
}
