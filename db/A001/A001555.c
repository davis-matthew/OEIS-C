/*
 * A001555  a(n) = 1^n + 2^n + ... + 8^n.
 * https://oeis.org/A001555
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1^n + 2^n + 3^n + 4^n + 5^n + 6^n + 7^n + 8^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   a001555_ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a001555_ipow.h"

seq_t a(idx_t n) {
    seq_t result = 0;
    int i;
    for (i = 1; i <= 8; i++) {
        result += a001555_ipow((seq_t)i, n);
    }
    return result;
}
