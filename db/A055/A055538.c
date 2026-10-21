/*
 * A055538  Number of asymmetric types of (3,n)-hypergraphs without isolated
 *          nodes, under action of symmetric group S_3; asymmetric n-covers of
 *          an unlabeled 3-set.
 * https://oeis.org/A055538
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 8, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   binomial, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A055538: Number of asymmetric types of (3,n)-hypergraphs without 
   isolated nodes, under action of symmetric group S_3. 
   The terms are calculated from the generating function

     G(x) = ( 1/(1-x)^8 - 3/(1-x)^4/(1-x^2)^2 + 2/(1-x)^2/(1-x^3)^2 
            - 3/(1-x)^4 + 3/(1-x)^2/(1-x^2) + 6/(1-x)^2 - 6/(1-x) ) / 6

   The coefficient of x^n in G(x) is a(n) for n ≥ 3.  The implementation
   expands each rational part into a finite sum, evaluates the sums, and
   combines them according to the formula above.
*/

#include "../utils/binomial.h"   /* binomial(idx_t a, idx_t b) */
#include "../utils/mul_small.h"  /* mul_small(seq_t x, unsigned int m) */

seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)0;          /* terms below the offset are undefined */

    seq_t c1, c2, c3, c4, c5, c6, c7, result;
    idx_t t, k, m;

    /* term 1: 1/(1-x)^8  → binom(n+7,7) */
    c1 = binomial(n + 7, 7);

    /* term 5: 3/(1-x)^2/(1-x^2)
       coefficient = (m+1)*(n+1-m)  where m = floor(n/2) */
    m   = n / 2;
    c5  = mul_small((seq_t)(m + 1),
                    (unsigned int)(n + 1 - m));

    /* term 2: -3/(1-x)^4/(1-x^2)^2
       coefficient = Σ_{t=0}^{⌊n/2⌋} (t+1)·binom(n-2t+3,3) */
    c2 = (seq_t)0;
    for (t = 0; 2 * t <= n; ++t) {
        seq_t bin = binomial(n - 2 * t + 3, 3);
        seq_t add = mul_small(bin, (unsigned int)(t + 1));
        c2 += add;
    }

    /* term 3: +2/(1-x)^2/(1-x^3)^2
       coefficient = Σ_{k=0}^{⌊n/3⌋} (k+1)·(n-3k+1) */
    c3 = (seq_t)0;
    for (k = 0; 3 * k <= n; ++k) {
        seq_t add = mul_small((seq_t)(n - 3 * k + 1),
                              (unsigned int)(k + 1));
        c3 += add;
    }

    /* term 4: -3/(1-x)^4  → -3·binom(n+3,3) */
    c4 = binomial(n + 3, 3);

    /* term 6: 6/(1-x)^2  → 6·(n+1) */
    c6 = mul_small((seq_t)(n + 1), 6);

    /* term 7: -6/(1-x)  → -6 */
    c7 = (seq_t)6;

    /* Assemble the numerator of G(x) */
    result = c1;
    result -= mul_small(c2, 3);
    result += mul_small(c3, 2);
    result -= mul_small(c4, 3);
    result += mul_small(c5, 3);
    result += c6;
    result -= c7;

    /* Exact division by 6 */
    result /= (seq_t)6;

    return result;
}
