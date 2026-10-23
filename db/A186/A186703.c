/*
 * A186703  Balanced ternary non-Harshad numbers.
 * https://oeis.org/A186703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th integer that is not divisible by the sum of its
 *            balanced ternary digits (unless the digit sum is 0). The value
 *            is found by brute‑force scan
 * Range:     u8 n <= 52, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   bal_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/bal_sum.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t m = 1;
    while (1) {
        int s = bal_sum(m);
        if (s == 0 || m % (seq_t)(s > 0 ? s : -s) != 0) {
            ++count;
            if (count == n) return m;
        }
        ++m;
    }
}
