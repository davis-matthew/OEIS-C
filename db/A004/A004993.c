/*
 * A004993  a(n) = (6^n/n!)*Product_{k=0..n-1} (6*k + 1).
 * https://oeis.org/A004993
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*(6*n-5)*a(n-1)/n, a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   a004993_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a004993_gcd.h"

seq_t a(idx_t n) {
    seq_t result = 1;
    for (idx_t i = 1; i <= n; i++) {
        idx_t num = (idx_t)6 * ((idx_t)6 * i - (idx_t)5);
        idx_t den = i;
        idx_t g = a004993_gcd(num, den);
        num /= g;
        den /= g;
        /* num and den are now coprime, so den divides result exactly */
        result /= (seq_t)den;
        result *= (seq_t)num;
    }
    return result;
}
