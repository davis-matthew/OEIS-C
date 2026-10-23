/*
 * A120812  Number of permutations of length n with exactly 4 occurrences of
 *          the pattern 2-13.
 * https://oeis.org/A120812
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (-36 - 100 n - 13 n^2 + 4 n^3 + n^4)/(24(n + 6)) *
 *            binom(2 n, n-5)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 15, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   binom, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"
#include "../utils/gcd.h"

/* offset: 5 */
seq_t a(idx_t n)
{
    /* a(n) is defined only for n >= 5; return 0 otherwise. */
    if (n < 5)
        return (seq_t)0;

    /* convert to seq_t for arithmetic */
    seq_t m = (seq_t)n;

    /* compute polynomial numerator: n^4 + 4 n^3 - 13 n^2 - 100 n - 36. */
    seq_t m2 = m * m;           /* n^2 */
    seq_t m3 = m2 * m;          /* n^3 */
    seq_t m4 = m3 * m;          /* n^4 */
    seq_t num = m4 + 4 * m3 - 13 * m2 - 100 * m - 36;

    /* denominator: 24 (n + 6). */
    seq_t denom = 24 * (m + 6);

    /* binomial coefficient binom(2n, n-5). */
    seq_t bin = binom(2 * (idx_t)n, n - 5);

    /* reduce common factors between numerator and denominator. */
    seq_t g = gcd(num, denom);
    num /= g;
    denom /= g;

    /* cancel any remaining factors of the denominator with the binomial. */
    g = gcd(bin, denom);
    bin /= g;
    denom /= g;

    /* at this point denom should be 1; guard against non‑unit denom. */
    seq_t result = bin * num;
    result /= denom;   /* safe when denom == 1 */

    return result;
}
