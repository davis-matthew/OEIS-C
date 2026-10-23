/*
 * A032854  Numbers whose base-7 representation Sum_{i=0..m} d(i)*7^i has d(m)
 *          <= d(m-1) >= d(m-2) <= ...
 * https://oeis.org/A032854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th smallest integer whose base‑7 representation
 *            satisfies d(m) ≤ d(m−1) ≥ d(m−2) ≤ d(m−3) … (alternating up‑down
 *            starting from the most significant digit)
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   a032854_valid
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/a032854_valid.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* defined only for n ≥ 1, guard */
    seq_t target = (seq_t)n;
    seq_t count = 0;
    seq_t val = 1;
    for (;; ++val) {
        if (a032854_valid(val)) {
            ++count;
            if (count == target) return val;
        }
    }
}
