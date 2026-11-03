/*
 * A083902  Number of divisors of n with the largest digit of the divisor <= 7
 *          (base 10).
 * https://oeis.org/A083902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of divisors d of n with all decimal digits ≤ 7
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Validated: partial, pass, 105 terms through n = 105 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

static bool digits_le7(uint64_t x) {
    if (x == 0) return true;          /* 0 has digit 0 ≤ 7 */
    while (x > 0) {
        if ((x % 10) > 7) return false;
        x /= 10;
    }
    return true;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;                 /* undefined for n < 1 */
    uint64_t nn = (uint64_t)n;
    seq_t count = 0;
    uint64_t d;
    for (d = 1; d * d <= nn; ++d) {
        if (nn % d == 0) {
            if (digits_le7(d)) count += 1;
            uint64_t d2 = nn / d;
            if (d2 != d && digits_le7(d2)) count += 1;
        }
    }
    return count;
}
