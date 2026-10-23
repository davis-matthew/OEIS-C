/*
 * A152876  Number of permutations of {1,2,...,n} having no consecutive
 *          triples of the form (odd, even, odd) or (even, odd, even).
 * https://oeis.org/A152876
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(2n) = (n!)^2 * Σ_{j=0..⌊n/2⌋} (C(n−j,j))^2 * *
 *            (2n^2−6jn+6j^2)/(n−j)^2 * a(2n+1) = n! (n+1)! * Σ_{j=0..⌊n/2⌋} *
 *            C(n+1−j,j) C(n−j,j) * * (2n^2+2n−6jn−3j+6j^2)/((n+1−j)(n−j))
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 22, u128 n <= 36,
 *            big any n
 * Helpers:   binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/factorial.h"

/* Returns the nth term of OEIS A152876 */
seq_t a(idx_t n)
{
    static const seq_t base[] = { (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)4 };
    if (n < 4)
        return base[n];

    if ((n & 1) == 0)   /* even n = 2*m */
    {
        idx_t m = n >> 1;
        seq_t fact_m = factorial(m);
        seq_t pref   = fact_m * fact_m;                 /* (m!)^2 */
        seq_t sum    = 0;
        idx_t maxj   = m / 2;
        for (idx_t j = 0; j <= maxj; ++j)
        {
            seq_t binom   = binomial(m - j, j);
            seq_t binom_sq = binom * binom;
            seq_t term_num = (seq_t)2 * (seq_t)m * (seq_t)m
                           - (seq_t)6 * (seq_t)j * (seq_t)m
                           + (seq_t)6 * (seq_t)j * (seq_t)j;
            seq_t denom = (seq_t)(m - j) * (seq_t)(m - j);
            seq_t cur   = binom_sq * term_num;
            cur = cur / denom;
            sum += cur;
        }
        return pref * sum;
    }
    else               /* odd n = 2*m + 1 */
    {
        idx_t m = (n - 1) >> 1;
        seq_t fact_m  = factorial(m);
        seq_t fact_m1 = factorial(m + 1);
        seq_t pref    = fact_m * fact_m1;      /* n! (n+1)! */
        seq_t sum     = 0;
        idx_t maxj    = m / 2;
        for (idx_t j = 0; j <= maxj; ++j)
        {
            seq_t binom_a   = binomial(m + 1 - j, j);
            seq_t binom_b   = binomial(m - j, j);
            seq_t binom_prod = binom_a * binom_b;
            seq_t term_num  = (seq_t)2 * (seq_t)m * (seq_t)m
                            + (seq_t)2 * (seq_t)m
                            - (seq_t)6 * (seq_t)j * (seq_t)m
                            - (seq_t)3 * (seq_t)j
                            + (seq_t)6 * (seq_t)j * (seq_t)j;
            seq_t denom = (seq_t)(m + 1 - j) * (seq_t)(m - j);
            seq_t cur   = binom_prod * term_num;
            cur = cur / denom;
            sum += cur;
        }
        return pref * sum;
    }
}
