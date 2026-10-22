/*
 * A004365  Binomial coefficient C(6n,n-10).
 * https://oeis.org/A004365
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(6*n, n-10)
 * Range:     u8 n <= 11, u16 n <= 12, u32 n <= 16, u64 n <= 24, u128 n <= 27,
 *            big any n
 * Helpers:   a004365_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a004365_gcd.h"

seq_t a(idx_t n) {
    idx_t k = n - 10;
    idx_t m = 6 * n;
    seq_t result = 1;
    idx_t i;

    /* result = C(m-k+i, i) after step i; reduce by gcd so that no
       intermediate exceeds the next exact term */
    for (i = 1; i <= k; i++) {
        idx_t num = m - k + i;
        idx_t den = i;
        idx_t g = a004365_gcd(num, den);
        num /= g;
        den /= g;
        result = result / (seq_t)den * (seq_t)num;
    }

    return result;
}
