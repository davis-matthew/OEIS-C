/*
 * A077641  Number of squarefree integers in closed interval [n, 2n-1], i.e.,
 *          among n consecutive numbers beginning with n.
 * https://oeis.org/A077641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{j=0}^{n-1} |μ(n+j)| (number of squarefree integers in
 *            [n,2n-1])
 * Range:     u8 n <= 217, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_squarefree.h"

seq_t a(idx_t n) {
    seq_t count = 0;
    idx_t limit = n + n - 1;   /* 2*n-1 */
    for (idx_t i = n; i <= limit; ++i) {
        if (is_squarefree((seq_t)i))
            ++count;
    }
    return count;
}
