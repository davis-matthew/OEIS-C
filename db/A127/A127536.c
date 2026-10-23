/*
 * A127536  Sum of jump-lengths of all even trees with 2n edges.
 * https://oeis.org/A127536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)(2n-1)*C(3n,n) / (3*(n+1)*(2n+1))
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Offset starts at 1: a(1) = 0 */
    if (n < 1)
        return 0;

    seq_t N = (seq_t)n;
    /* binomial(3n, n) */
    seq_t bin = binomial(3 * n, n);

    /* numerator part (n-1)(2n-1) */
    seq_t num = (N - 1) * (2 * N - 1);

    /* denominator part 3*(n+1)*(2n+1) */
    seq_t den = 3 * (N + 1) * (2 * N + 1);

    /* Reduce gcd between bin and den */
    seq_t g = gcd(bin, den);
    if (g > 1) {
        bin /= g;
        den /= g;
    }

    /* Reduce gcd between num and den */
    g = gcd(num, den);
    if (g > 1) {
        num /= g;
        den /= g;
    }

    /* Now compute result */
    seq_t res = bin * num;
    if (den != 1)
        res /= den;   /* exact division */

    return res;
}
