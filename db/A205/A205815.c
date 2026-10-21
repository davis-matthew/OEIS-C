/*
 * A205815  a(n) = Sum_{k=1..n} binomial(n,k) * sigma(n,k) * 2^(n-k).
 * https://oeis.org/A205815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n} ((d+2)^n - 2^n)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/pow2.h"      /* seq_t pow2(idx_t);      */
#include "../utils/big_pow.h"   /* seq_t big_pow(seq_t, seq_t); */

/* Computes the sequence element a(n) for n ≥ 1 (offset = 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* the sequence is defined starting at n=1 */

    seq_t result = 0;
    seq_t pow2_n = pow2(n);

    /* Iterate over all divisors d of n in a single pass. */
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            /* divisor d */
            seq_t term = big_pow((seq_t)(d + 2), (seq_t)n) - pow2_n;
            result += term;

            /* paired divisor n/d, if distinct from d */
            idx_t d2 = n / d;
            if (d2 != d) {
                seq_t term2 = big_pow((seq_t)(d2 + 2), (seq_t)n) - pow2_n;
                result += term2;
            }
        }
    }
    return result;
}
