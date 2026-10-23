/*
 * A017741  Binomial coefficients C(n,77).
 * https://oeis.org/A017741
 *
 * Offset:    77    (a(77) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial coefficient C(n,77)
 * Range:     u8 n <= 78, u16 n <= 79, u32 n <= 83, u64 n <= 95, u128 n <= 133,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    const idx_t k = 77;
    if (n < k) return 0;

    /* Compute C(n,k) multiplicatively while keeping intermediates small:
       result = 1
       for i = 1..k
           num = n - k + i
           den = i
           g   = gcd(den, result)
           den /= g
           result /= g
           result *= num
           result /= den   (exact division)
    */
    seq_t result = 1;
    const idx_t n_minus_k = n - k;

    for (idx_t i = 1; i <= k; ++i) {
        seq_t num = (seq_t)(n_minus_k + i);
        seq_t den = (seq_t)i;

        seq_t g = gcd(den, result);
        den /= g;
        result /= g;

        result *= num;
        result /= den;
    }

    return result;
}
