/*
 * A205824  (3n)!/[3n*n!*(n+1)!].
 * https://oeis.org/A205824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3n)!/(3n*n!*(n+1)!)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)1;
    seq_t result = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i) {
        /* numerator = (3i-1)*(3i-2)*(3i-3)   */
        seq_t t = (seq_t)(3 * i);
        seq_t N = (t - 1) * (t - 2) * (t - 3);

        /* denominator = (i+1)*i */
        seq_t D = (seq_t)(i + 1) * (seq_t)i;

        /* cancel common factors between result and denominator */
        seq_t g = gcd(result, D);
        result /= g;
        D /= g;

        /* cancel remaining factors from numerator */
        g = gcd(N, D);
        N    /= g;
        D    /= g;       /* D should now be 1 */

        /* multiply by the reduced numerator */
        result = result * N;
    }
    return result;
}
