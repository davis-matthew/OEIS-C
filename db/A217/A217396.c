/*
 * A217396  Prime indices of Euler's lucky numbers.
 * https://oeis.org/A217396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, less
 * Formula:   a(n) = A000720(L(n)), where L(n) is the n-th Euler lucky number:
 *            n with m^2-m+n prime for m=0..n-1 (found by search; only 6
 *            exist: 2,3,5,11,17,41)
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Validated: partial, pass, 6 terms through n = 6 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool lucky_is_prime(unsigned long x) {
    if (x < 2) return false;
    for (unsigned long d = 2; d * d <= x; d++)
        if (x % d == 0) return false;
    return true;
}

static bool lucky_ok(unsigned long k) {
    for (unsigned long m = 0; m < k; m++)
        if (!lucky_is_prime(m * m - m + k + (m == 0 ? 0 : 0))) return false;
    return true;
}

seq_t a(idx_t n) {
    if (n == 0 || n > 6) return 0;
    idx_t found = 0;
    for (unsigned long k = 2; k <= 41; k++) {
        if (lucky_ok(k)) {
            found++;
            if (found == n) {
                seq_t cnt = 0;
                for (unsigned long i = 2; i <= k; i++)
                    if (lucky_is_prime(i)) cnt++;
                return cnt;
            }
        }
    }
    return 0;
}
