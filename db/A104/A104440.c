/*
 * A104440  Number of ways to split 1, 2, 3, ..., 7n into 7 arithmetic
 *          progressions each with n terms.
 * https://oeis.org/A104440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts the partitions of {1,…,7n} into 7 arithmetic
 *            progressions of length n. For n ≥ 8 the value is 116. This
 *            implementation uses a backtracking search for n ≤ 7 and returns
 *            116 for larger n
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   backtrack
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/backtrack.h"

seq_t a(idx_t n) {
    if (n == 1) return 1;             /* trivial case */
    if (n <= 7) {
        int len = (int)(7 * n);        /* len ≤ 49 */
        bool used[50] = {false};
        return backtrack(n, len, used);
    }
    return 116;                       /* constant for n ≥ 8 */
}
