/*
 * A190668  a(n) = Product_{k>=1} floor(n^(1/k)).
 * https://oeis.org/A190668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{k>=1} floor(n^(1/k))
 * Range:     u8 n <= 15, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   _floor_kth_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/_floor_kth_root.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* not used, but defined for safety */

    seq_t product = (seq_t)1;
    for (idx_t k = 1;; ++k) {
        idx_t root = _floor_kth_root(n, k);
        if (root <= 1) break;          /* remaining terms would be 1 */
        product *= (seq_t)root;
    }
    return product;
}
