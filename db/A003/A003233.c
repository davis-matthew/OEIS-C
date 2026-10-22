/*
 * A003233  Numbers k such that A003231(A001950(k)) = A001950(A003231(k)).
 * https://oeis.org/A003233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: k is a term iff A003231(A001950(k)) =
 *            A001950(A003231(k)), with A001950(m) = m + floor(m*phi) =
 *            floor((3m + sqrt(5m^2))/2) and A003231(m) =
 *            floor(m*(5+sqrt(5))/2) = floor((5m + sqrt(5m^2))/2), evaluated
 *            exactly using integer square roots
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   r3233_a1950, r3233_a3231, r3233_isqrt, r3233_s
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/r3233_a1950.h"
#include "../utils/r3233_a3231.h"
#include "../utils/r3233_isqrt.h"
#include "../utils/r3233_s.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    uint64_t k = 0;
    while (count < n) {
        k++;
        uint64_t lhs = r3233_a3231(r3233_a1950(k));
        uint64_t rhs = r3233_a1950(r3233_a3231(k));
        if (lhs == rhs) count++;
    }
    return (seq_t)k;
}
