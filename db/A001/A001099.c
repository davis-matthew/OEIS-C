/*
 * A001099  a(n) = n^n - a(n-1), with a(1) = 1.
 * https://oeis.org/A001099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^n - a(n-1), a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Helpers:   a001099_powi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a001099_powi.h"

static idx_t a001099_last_n = 0;
static seq_t a001099_last_v = 0;

seq_t a(idx_t n) {
    idx_t i;
    seq_t result;
    if (n < 1) return 0;
    if (a001099_last_n >= 1 && a001099_last_n <= n) {
        i = a001099_last_n;
        result = a001099_last_v;
    } else {
        i = 1;
        result = 1; /* a(1) = 1 */
    }
    while (i < n) {
        i++;
        result = a001099_powi((seq_t)i, i) - result;
    }
    a001099_last_n = n;
    a001099_last_v = result;
    return result;
}
