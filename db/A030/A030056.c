/*
 * A030056  a(n) = binomial(2*n+1, n-6).
 * https://oeis.org/A030056
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(2*n+1, n-6) (offset 6)
 * Range:     u8 n <= 8, u16 n <= 11, u32 n <= 18, u64 n <= 34, u128 n <= 65,
 *            big any n
 * Helpers:   _seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_seq_gcd.h"

/* Euclidean GCD for seq_t.  Allowed operations: %, /, *, +, -, comparisons. */

/* a(n) = binomial(2*n+1, n-6) for n >= 6, 0 otherwise. */
seq_t a(idx_t n) {
    if (n < 6)               /* undefined below the offset */
        return 0;

    idx_t k = n - 6;          /* lower index in the binomial */
    seq_t result = 1;
    seq_t m = (seq_t)2 * n + 1;   /* upper index of the binomial */

    for (idx_t i = 1; i <= k; ++i) {
        seq_t num = m - k + i;  /* numerator term */
        seq_t den = i;          /* denominator term */

        seq_t g = _seq_gcd(num, den);
        num /= g;               /* remove common factors from num and den */
        den /= g;

        g = _seq_gcd(result, den);
        result /= g;           /* remove remaining common factors from result and den */
        den /= g;              /* now den should be 1 */

        result *= num;          /* safe multiplication after reductions */
    }

    return result;
}
