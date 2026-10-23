/*
 * A118017  Largest denominators in even Egyptian fraction representation of
 *          1.
 * https://oeis.org/A118017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 12, a(n+1) = a(n) + a(n)^2/2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
seq_t a(idx_t n) {
    if (n == 0) return 0;
    seq_t res = 12;
    if (n == 1) return res;
    for (idx_t i = 2; i <= n; ++i) {
        res = res + (res * res) / 2;
    }
    return res;
}
