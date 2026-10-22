/*
 * A004324  Binomial coefficient C(3n,n-6).
 * https://oeis.org/A004324
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(3n, n-6) = Product_{i=0..n-7} (3n-i)/(i+1)
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 15, u64 n <= 27, u128 n <= 50,
 *            big any n
 * Helpers:   a004324_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a004324_gcd.h"

seq_t a(idx_t n) {
    if (n < 6) return 0;

    seq_t result = 1;
    idx_t k = n - 6;
    for (idx_t i = 0; i < k; i++) {
        seq_t num = (seq_t)3 * n - i;
        seq_t den = (seq_t)i + 1;
        /* result * num is divisible by den; cancel common factors first
           so the intermediate never exceeds the next binomial value */
        seq_t g = a004324_gcd(result, den);
        result /= g;
        den /= g;
        num /= den;
        result *= num;
    }

    return result;
}
