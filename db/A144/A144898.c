/*
 * A144898  Expansion of x/((1-x-x^3)*(1-x)^4).
 * https://oeis.org/A144898
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{j=0..floor((n+3)/3)} binomial(n-2*j+3, j+4)
 * Range:     u8 n <= 6, u16 n <= 19, u32 n <= 48, u64 n <= 106, u128 n <= 222,
 *            big any n
 * Helpers:   seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Author: adapted for arbitrary‑precision seq_t. */

#include <stdint.h>
#include "../utils/seq_gcd.h"

/* multiplicative binomial computation that never overflows seq_t
 * (uses successive gcd reductions). */
static seq_t binom_small(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;       /* use symmetry */

    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t num = n - k + i;       /* positive */
        seq_t den = i;

        /* reduce num/den by their gcd */
        seq_t g1 = seq_gcd(num, den);
        num /= g1;
        den /= g1;

        /* reduce res/den by their gcd */
        seq_t g2 = seq_gcd(res, den);
        res /= g2;
        den /= g2;                   /* den should now be 1 */

        /* multiply */
        res *= num;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t limit = (n + 3) / 3;           /* floor((n+3)/3) */

    for (idx_t j = 0; j <= limit; ++j) {
        /* n-2*j+3 = (n+3) - 2*j  (no underflow) */
        idx_t nm = (n + 3) - 2 * j;       /* n - 2*j + 3 */
        idx_t k  = j + 4;                /* j + 4 */

        sum += binom_small(nm, k);
    }
    return sum;
}
