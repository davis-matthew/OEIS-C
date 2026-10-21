/*
 * A081863  Largest integer m such that m divides
 *          (sigma_(2n+1)(2k-1)-sigma(2k-1)) for all k>=1.
 * https://oeis.org/A081863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(3+v₂(n)) * ∏{q prime, (q-1) | 2n, q > 2} q
 * Range:     u8 n <= 3, u16 n <= 17, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the largest integer m such that m | (σ_{2n+1}(2k-1)-σ(2k-1))  ∀k≥1 */

#include <stdbool.h>
#include "../utils/is_prime.h"

/* Return a(n) for n >= 1. For n == 0 the value is undefined; return 0. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Count trailing zeros of n (v₂(n)) */
    idx_t tmp = n;
    idx_t v2 = 0;
    while ((tmp & 1ULL) == 0) {
        ++v2;
        tmp >>= 1;
    }

    /* Compute 2^(3+v2) */
    seq_t result = 1;
    idx_t pow2 = 3 + v2;
    for (idx_t i = 0; i < pow2; ++i) {
        result *= (seq_t)2;
    }

    /* L = 2n */
    idx_t L = n << 1;      /* since n>=1, L>0 */

    /* Enumerate divisors d of L, d >= 2 (since d=1 gives q=2 which is excluded) */
    for (idx_t d = 1; d * d <= L; ++d) {
        if (L % d == 0) {
            /* first divisor d */
            idx_t q = d + 1;
            if (q > 2 && is_prime(q)) {
                result *= (seq_t)q;
            }

            /* complementary divisor d2 = L/d, avoid double count */
            idx_t d2 = L / d;
            if (d2 != d) {
                idx_t q2 = d2 + 1;
                if (q2 > 2 && is_prime(q2)) {
                    result *= (seq_t)q2;
                }
            }
        }
    }

    return result;
}
