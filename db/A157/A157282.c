/*
 * A157282  Maximum cardinality of a weakly triple-free subset of {1, 2, ...,
 *          n}.
 * https://oeis.org/A157282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: greedily build a maximum weakly triple‑free subset of {1,…,n}
   by processing numbers in decreasing order.  For each i, include i unless
   both 2i and 3i are already chosen.  This yields the maximum possible size. */
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                       /* offset 1 */
    bool *sel = (bool *)malloc((n + 1) * sizeof(bool));
    if (!sel) return 0;                         /* allocation failure */
    memset(sel, 0, (n + 1) * sizeof(bool));

    seq_t count = 0;
    for (idx_t i = n; i >= 1; --i) {
        idx_t i2 = i * 2;
        idx_t i3 = i * 3;
        bool skip = false;
        if (i2 <= n && sel[i2]) {
            if (i3 <= n && sel[i3]) skip = true;
        }
        if (!skip) {
            sel[i] = true;
            ++count;
        }
    }
    free(sel);
    return count;
}
