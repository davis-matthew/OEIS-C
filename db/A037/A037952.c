/*
 * A037952  a(n) = binomial(n, floor((n-1)/2)).
 * https://oeis.org/A037952
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n, floor((n-1)/2))
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 67, u128 n <= 131,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"

/* multiplicative binomial implementation that never overflows
   seq_t is assumed to be an unsigned integer type or an
   arbitrary‑precision class; intermediate steps are kept
   within the range of seq_t. */
static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;          /* use symmetry */
    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* current numerator and denominator */
        seq_t num = (seq_t)n - i + 1;  /* n, n-1, ..., n-k+1 */
        seq_t den = i;
        /* cancel common factors between num and den */
        seq_t g = gcd(num, den);
        num /= g;
        den /= g;
        /* cancel any remaining factors of den with result */
        g = gcd(result, den);
        result /= g;
        den /= g;                      /* den now divides result */
        /* multiply the reduced numerator */
        result = result * num;         /* product stays <= final binomial */
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    idx_t k = (n - 1) / 2;              /* floor((n-1)/2) */
    return binom(n, k);
}
