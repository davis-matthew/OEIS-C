/*
 * A004330  Binomial coefficient C(3n,n-12).
 * https://oeis.org/A004330
 *
 * Offset:    12    (a(12) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(3n, n-12) = Product_{i=0..n-13} (3n-i)/(i+1)
 * Range:     u8 n <= 13, u16 n <= 15, u32 n <= 20, u64 n <= 30, u128 n <= 53,
 *            big any n
 * Helpers:   gcd_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd_seq.h"

seq_t a(idx_t n) {
    if (n < 12) return 0;

    seq_t result = 1;
    idx_t k = n - 12;

    for (idx_t i = 0; i < k; i++) {
        seq_t m = (seq_t)(3 * n - i);
        seq_t d = (seq_t)(i + 1);
        /* result * m is divisible by d; split the division so that the
           intermediate never exceeds the final product. */
        seq_t g = gcd_seq(result, d);
        result /= g;
        d /= g;
        /* now gcd(result, d) = 1, so d divides m */
        m /= d;
        result *= m;
    }

    return result;
}
