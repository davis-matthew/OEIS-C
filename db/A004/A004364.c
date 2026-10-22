/*
 * A004364  Binomial coefficient C(6n,n-9).
 * https://oeis.org/A004364
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(6n, n-9) = Product_{i=1..n-9} (5n+9+i)/i
 * Range:     u8 n <= 10, u16 n <= 12, u32 n <= 15, u64 n <= 23, u128 n <= 26,
 *            big any n
 * Helpers:   a004364_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a004364_gcd.h"

seq_t a(idx_t n) {
    idx_t m = 6 * n;
    idx_t k = n - 9;
    seq_t result = 1;

    for (idx_t i = 0; i < k; i++) {
        idx_t num = m - i;
        idx_t den = i + 1;
        idx_t g = a004364_gcd(num, den);
        num /= g;
        den /= g;
        /* result * num is divisible by den and gcd(num, den) = 1,
           so den divides result exactly */
        result /= den;
        result *= num;
    }

    return result;
}
