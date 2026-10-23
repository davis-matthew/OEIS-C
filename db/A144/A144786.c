/*
 * A144786  If n is an oblong number A002378, then a(n)=a(j) where j is the
 *          number of oblong numbers in (0,n], otherwise a(n)=n.
 * https://oeis.org/A144786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n if n is not an oblong (pronic) number; otherwise a(n) =
 *            a(k) where k = # of oblong numbers ≤ n
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   count_oblong
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdbool.h>
#include "../utils/count_oblong.h"

seq_t a(idx_t n) {
    while (true) {
        idx_t m = count_oblong(n);
        if (m == 0) break;                      /* no oblongs ≤ n, so n is not oblong */
        if (n == m * (m + 1)) {                /* n itself is oblong */
            n = m;                              /* recurse on the count */
            continue;
        }
        break;                                  /* n is not oblong */
    }
    return (seq_t)n;
}
