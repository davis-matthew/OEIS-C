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
 * Validated: standard, pass, 37 terms through n = 464 (2026-11-01)
 * Helpers:   binomial, pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"
#include "../utils/pow3.h"

static inline seq_t pow4(idx_t exp) {
    seq_t result = 1;
    seq_t base   = 4;
    idx_t e = exp;
    while (e) {
        if (e & 1) result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}

static inline seq_t power5(idx_t exp)
{
    seq_t result = 1;
    seq_t base   = 5;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

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
