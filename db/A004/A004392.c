/*
 * A004392  Binomial coefficient C(8n,n-11).
 * https://oeis.org/A004392
 *
 * Offset:    11    (a(11) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(8n, n-11) = Product_{i=1..n-11} (7n+11+i)/i
 * Range:     u8 n <= 12, u16 n <= 13, u32 n <= 16, u64 n <= 23, u128 n <= 38,
 *            big any n
 * Helpers:   a004392_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a004392_gcd.h"

seq_t a(idx_t n) {
    seq_t result = 1;
    idx_t k = n - 11;
    idx_t i;

    for (i = 1; i <= k; i++) {
        seq_t num = (seq_t)(8 * n - i + 1);
        seq_t den = (seq_t)i;
        seq_t g = a004392_gcd(result, den);
        result /= g;
        den /= g;
        /* den now divides num exactly */
        num /= den;
        result *= num;
    }

    return result;
}
