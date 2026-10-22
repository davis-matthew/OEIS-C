/*
 * A004311  Binomial coefficient C(2n,n-5).
 * https://oeis.org/A004311
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(2n, n-5), computed multiplicatively as prod_{i=1..n-5}
 *            (2n-i+1)/i with gcd reduction
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 18, u64 n <= 34, u128 n <= 66,
 *            big any n
 * Helpers:   a004311_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a004311_gcd.h"

seq_t a(idx_t n)
{
    seq_t result = 1;
    if (n < 5)
        return 0;
    idx_t k = n - 5;
    for (idx_t i = 1; i <= k; i++) {
        seq_t m = (seq_t)(2 * n - i + 1);
        seq_t d = (seq_t)i;
        seq_t g = a004311_gcd(result, d);
        result /= g;
        d /= g;
        m /= d;
        result *= m;
    }
    return result;
}
