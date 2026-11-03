/*
 * A239061  Number of integers x, 1 <= x <= n, such that x^x == 1 (mod n).
 * https://oeis.org/A239061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{x : 1 ≤ x ≤ n, x^x ≡ 1 (mod n)}
 * Range:     u8 n <= 1494, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 9243 (2026-11-01)
 * Helpers:   powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/powmod.h"

seq_t a(idx_t n) {
    if (n == (idx_t)1)
        return (seq_t)1;

    seq_t count   = 0;
    seq_t mod     = (seq_t)n;

    for (idx_t x = 1; x <= n; ++x) {
        seq_t rem = powmod((seq_t)x, (uint64_t)x, mod);
        if (rem == (seq_t)1)
            ++count;
    }
    return count;
}
