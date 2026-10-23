/*
 * A015376  Gaussian binomial coefficient [ n,9 ] for q=-4.
 * https://oeis.org/A015376
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Product_{i=1..9} ((-4)^(n-i+1)-1)/((-4)^i-1)
 * Range:     i8 n <= 9, i16 n <= 9, i32 n <= 10, i64 n <= 12, i128 n <= 16,
 *            big any n
 * Helpers:   gcd, pow_neg4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/pow_neg4.h"

/* Gaussian binomial coefficient [n,9] for q = -4 */
seq_t a(idx_t n)
{
    if (n < 9) return 0;   /* values below the offset are undefined */

    seq_t bin[10] = {0};   /* bin[k] holds [current-1][k] during the loop */
    bin[0] = 1;            /* [m,0] = 1 for all m */

    for (idx_t current = 1; current <= n; ++current) {
        idx_t kmax = current < 9 ? current : 9;
        for (idx_t k = kmax; k >= 1; --k) {
            /* add = (-4)^{current-k} * bin[k-1]   */
            seq_t add = pow_neg4(current - k) * bin[k-1];
            bin[k] += add;
        }
    }
    return bin[9];
}
