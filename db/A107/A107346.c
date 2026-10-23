/*
 * A107346  Differences between successive permutations of 1,2,3,4,5 regarded
 *          as decimal numbers arranged in increasing order.
 * https://oeis.org/A107346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base, fini, full
 * Formula:   a(n) = difference between the n-th and (n-1)-th permutations *
 *            of the digits 1,2,3,4,5 interpreted as decimal numbers sorted in
 *            ascending order
 * Range:     u8 n <= 5, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   _init_diffs, _perm_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/_init_diffs.h"
#include "../utils/_perm_value.h"

seq_t a(idx_t n) {
    static seq_t diffs[119] = {0};
    static int init = 0;
    if (!init) {
        _init_diffs(diffs);
        init = 1;
    }
    if (n == 0) return 0;
    if (n > 119) return 0;
    return diffs[n - 1];
}
