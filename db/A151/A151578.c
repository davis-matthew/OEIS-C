/*
 * A151578  Number of permutations of 1..n arranged in a circle with exactly 5
 *          adjacent element pairs in decreasing order.
 * https://oeis.org/A151578
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(5^(n-1) - n*4^(n-1) + binomial(n,2)*3^(n-1) -
 *            binomial(n,3)*2^(n-1) + binomial(n,4))
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   binomial, pow2, pow3, pow4, power5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"
#include "../utils/pow3.h"
#include "../utils/pow4.h"
#include "../utils/power5.h"

seq_t a(idx_t n)
{
    if (n < 5) return 0;          /* offset starts at 5 */

    seq_t N = (seq_t)n;

    seq_t inner = power5(n - 1);                /* 5^(n-1)           */
    inner -= N * pow4(n - 1);                  /* - n*4^(n-1)        */
    inner += binomial(n, 2) * pow3(n - 1);     /* + C(n,2)*3^(n-1)   */
    inner -= binomial(n, 3) * pow2(n - 1);     /* - C(n,3)*2^(n-1)   */
    inner += binomial(n, 4);                   /* + C(n,4)           */

    return N * inner;                          /* multiply by n       */
}
