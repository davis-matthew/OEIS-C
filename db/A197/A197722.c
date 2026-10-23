/*
 * A197722  Smallest number k such that k^2 begins with n^3.
 * https://oeis.org/A197722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest integer k with k^2 starting with n^3
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 22, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   floor_sqrt, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow10.h"      /* pow10(seq_t exponent) */
#include "../utils/floor_sqrt.h" /* floor_sqrt(seq_t x) */

seq_t a(idx_t n) {
    /* Offset 1: a(1) is defined. For n == 0 return 0 (undefined). */
    if (n == 0) return 0;

    seq_t sn = (seq_t)n;          /* n as seq_t */
    seq_t p  = sn * sn * sn;      /* n^3 */
    seq_t p1 = p + 1;             /* p + 1 */

    seq_t pow10 = 1;              /* 10^m */
    for (idx_t m = 0; ; ++m) {
        /* Compute lower = p * 10^m and upper = (p+1) * 10^m - 1 */
        seq_t lower = p * pow10;
        /* Detect overflow of multiplication; if overflow, stop searching */
        if (p != 0 && lower / p != pow10) break;

        seq_t upper = (p1) * pow10;
        if (p1 != 0 && upper / p1 != pow10) break;
        if (upper == 0) break;          /* should not happen for n >= 1 */
        upper = upper - 1;              /* upper = (p1)*10^m - 1 */

        /* Find ceil(sqrt(lower)) */
        seq_t s = floor_sqrt(lower);
        seq_t k = (s * s == lower) ? s : s + 1;

        /* Check if k^2 <= upper without overflow */
        if (k != 0 && k <= upper / k) {
            return k;
        }

        /* Prepare next pow10 = 10^(m+1) */
        seq_t next_pow10 = pow10 * 10;
        if (next_pow10 / 10 != pow10) break;   /* overflow, stop */
        pow10 = next_pow10;
    }

    /* In the unlikely event no k found before overflow, return 0 */
    return 0;
}
