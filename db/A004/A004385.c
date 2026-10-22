/*
 * A004385  Binomial coefficient C(8n,n-4).
 * https://oeis.org/A004385
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(8*n, n-4) = Product_{i=1..n-4} (7*n+4+i)/i
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 18, u128 n <= 22,
 *            big any n
 * Helpers:   a004385_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a004385_gcd.h"

seq_t a(idx_t n) {
    idx_t k = n - 4;
    idx_t base = 7 * n + 4;
    seq_t result = 1;

    for (idx_t i = 1; i <= k; i++) {
        seq_t num = (seq_t)(base + i);
        seq_t den = (seq_t)i;
        seq_t g = a004385_gcd(result, den);
        result /= g;
        den /= g;
        num /= den;
        result *= num;
    }

    return result;
}
