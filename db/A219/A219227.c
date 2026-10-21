/*
 * A219227  a(n) is the sum of n addends nested as follows:
 *          floor(f(floor(f(...(n)...)))) where f(x) = x^(1/3).
 * https://oeis.org/A219227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{n} f^k(n), where f(x) = ⌊x^(1/3)⌋ (the same
 *            squares floor operation is used at every step)
 * Range:     u8 n <= 63, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_cube_root.h"

seq_t a(idx_t n)
{
    /* Handle the offset: a(1) is the first term. */
    if (n == 0)
        return 0;                   /* undefined for the sequence, safe fallback */

    seq_t sum = 0;
    seq_t cur = n;

    /* Iterate exactly n times, computing the nested floor cube roots. */
    for (idx_t i = 0; i < n; ++i) {
        cur = floor_cube_root(cur);  /* f(cur) */
        sum += cur;

        if (cur == 1) {
            /* Remaining terms are all 1.  Subtract the already processed term. */
            sum += (n - 1 - i);
            break;
        }
    }

    return sum;
}
