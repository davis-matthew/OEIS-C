/*
 * A078692  Triangle reads by rows: T(n,k) = coefficient of x^k in
 *          (x^3-2*x^2-2*x+1)^n.
 * https://oeis.org/A078692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabf
 * Formula:   a(n) = coefficient of x^k in (1 - 2x - 2x^2 + x^3)^r, where
 *            (r,k) is the entry of the triangle T(r,k) that is flattened
 *            row‑wise. For n = 1 the first term of row 1, for n = 2 the
 *            second term of row 1, …, for n = 4 the last term of row 1, for n
 *            = 5 the first term of row 2, and so on
 * Range:     i8 n <= 27, i16 n <= 126, i32 n <= 397, i64 n <= 1560,
 *            i128 n <= 5825, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
/* The binomial helper is provided by the build system.  */
#include "../utils/binom.h"

/* Return 2^e as a seq_t.  The loop multiplies with '2' each
   iteration, so big intermediate values are allowed if seq_t
   is an arbitrary‑precision type. */
static seq_t pow2_seq(idx_t e)
{
    seq_t res = 1;
    for (idx_t i = 0; i < e; ++i)
        res *= (seq_t)2;
    return res;
}

/* Compute the nth term of A078692.  */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for n<1, but safe return */

    /* Find the row r such that the cumulative number of terms
       up to row r is at least n.  The cumulative count up to row
       r is (3r^2 + 5r)/2.  */
    idx_t r = 0;
    while ((3 * r * r + 5 * r) / 2 < n)
        ++r;

    /* Number of terms prior to row r. */
    idx_t prev = (3 * (r - 1) * (r - 1) + 5 * (r - 1)) / 2;

    /* Position k within row r (0‑based).  The row has 3r+1 entries. */
    idx_t k = n - prev - 1;          /* 0 <= k < 3r+1 */

    seq_t total = 0;

    /* Sum over all triples (i3,i2,i1) that produce exponent k.
       i0 is implied as r - i1 - i2 - i3. */
    for (idx_t i3 = 0; i3 <= r && 3 * i3 <= k; ++i3) {
        for (idx_t i2 = 0; i2 <= r - i3 && 2 * i2 + 3 * i3 <= k; ++i2) {
            idx_t i1 = k - 2 * i2 - 3 * i3;
            if (i1 > r - i3 - i2)
                continue;
            idx_t i0 = r - i3 - i2 - i1;   /* always >= 0 */

            /* Multinomial coefficient r!/(i0!i1!i2!i3!) expressed as
               a product of binomials to keep the intermediate values
               reasonably sized.  The helper binom() returns seq_t
               and works for arbitrary precision. */
            seq_t ways = binom(r, i3);
            ways = ways * binom(r - i3, i2);
            ways = ways * binom(r - i3 - i2, i1);

            /* Factor (-2)^(i1+i2) = (-1)^(i1+i2) * 2^(i1+i2).  */
            idx_t exp = i1 + i2;
            seq_t pow2 = pow2_seq(exp);
            if (exp & 1)
                pow2 = -pow2;          /* apply the sign */

            seq_t term = ways * pow2;
            total = total + term;
        }
    }

    return total;
}
