/*
 * A003252  The number m such that A003251(m) = A003231(n).
 * https://oeis.org/A003252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3n - j(n), where j(n) = #{k >= 1 : A003249(k) <= n},
 *            A003249(k) = A001950(A003234(k)) + 1, and A003234 = numbers m
 *            with A003231(A001950(m)) = A001950(A003231(m)) - 1 (Carlitz,
 *            Theorem 7.15)
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   a3252_B, a3252_U, a3252_isqrt, a3252_lower
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a3252_B.h"
#include "../utils/a3252_U.h"
#include "../utils/a3252_isqrt.h"
#include "../utils/a3252_lower.h"

seq_t a(idx_t n) {
    uint64_t j = 0;
    for (uint64_t m = 1; a3252_U(m) + 1 <= (uint64_t)n; m++) {
        if (a3252_B(a3252_U(m)) + 1 == a3252_U(a3252_B(m)))
            j++;
    }
    return (seq_t)n * 3 - (seq_t)j;
}
