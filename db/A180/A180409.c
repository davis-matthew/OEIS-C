/*
 * A180409  Unique digits used in n in numerical order (with 0 last).
 * https://oeis.org/A180409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = unique digits of n sorted ascending, zero last
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    bool used[10] = {false};
    idx_t t = n;
    while (t > 0) {
        unsigned d = t % 10;
        used[d] = true;
        t /= 10;
    }
    seq_t res = 0;
    for (unsigned d = 1; d <= 9; ++d) {
        if (used[d]) {
            res = res * 10 + (seq_t)d;
        }
    }
    if (used[0]) {
        res = res * 10;
    }
    return res;
}
