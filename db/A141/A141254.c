/*
 * A141254  Number of permutations that lie in the cyclic closure of Av(123) -
 *          i.e., at least one cyclic rotation of the permutation avoids the
 *          pattern 123.
 * https://oeis.org/A141254
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * (C(n) - 2^n + binomial(n,2) + 2) for n ≥ 4 where C(n)
 *            is the nth Catalan number. The first four terms are defined
 *            explicitly: a(0)=1, a(1)=1, a(2)=2, a(3)=6
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Validated: partial, pass, 25 terms through n = 24 (2026-11-01)
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Explicit values for n < 4 */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)6;

    /* Catalan(n) = binomial(2n, n) / (n+1) */
    seq_t catalan = binomial((idx_t)(2 * n), n);
    catalan = catalan / ((seq_t)n + (seq_t)1);

    seq_t pow2n     = pow2(n);          /* 2^n   */
    seq_t binom_n2  = binomial(n, 2);   /* nC2   */

    seq_t inside = catalan - pow2n + binom_n2 + (seq_t)2;
    return (seq_t)n * inside;
}
