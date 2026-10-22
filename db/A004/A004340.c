/*
 * A004340  Binomial coefficient C(4n,n-10).
 * https://oeis.org/A004340
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(4*n, n-10)
 * Range:     u8 n <= 11, u16 n <= 13, u32 n <= 17, u64 n <= 26, u128 n <= 46,
 *            big any n
 * Helpers:   a004340_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a004340_gcd.h"

seq_t a(idx_t n)
{
    idx_t m = 4 * n;
    idx_t k = n - 10;

    if (k > m - k)
        k = m - k;

    seq_t result = 1;
    for (idx_t i = 0; i < k; i++) {
        /* result * (m - i) / (i + 1) is exact; divide first to avoid overflow */
        seq_t num = (seq_t)(m - i);
        seq_t den = (seq_t)(i + 1);
        seq_t g = a004340_gcd(result, den);
        result /= g;
        den /= g;
        num /= den;
        result *= num;
    }

    return result;
}
