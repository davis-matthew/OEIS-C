/*
 * A025542  (1/binomial(0,0) - 1/binomial(2,1) + ... + d/binomial(2n,n))*L,
 *          where d = (-1)^n, L = LCM{binomial(0,0), binomial(2,1),...,
 *          binomial(2n,n)}.
 * https://oeis.org/A025542
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = L * Σ_{k=0..n} (-1)^k / binom(2k,k) where L = lcm{
 *            binom(2k,k) | 0 ≤ k ≤ n } and binom is the central binomial
 *            coefficient C(2k,k). The result is always an integer
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Step 1: compute the lcm L of all central binomial coefficients
       up to n. */
    seq_t L = (seq_t)1;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t c = binomial((idx_t)(2 * k), k);       /* binom(2k,k) */
        seq_t g = gcd(L, c);                           /* gcd(L,c) */
        L = (L / g) * c;                               /* lcm(L,c) */
    }

    /* Step 2: evaluate the alternating sum with the common denominator L.
       Each term is simply L / binom(2k,k).  Multiplying by L later
       yields the integer a(n). */
    seq_t result = (seq_t)0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t c   = binomial((idx_t)(2 * k), k);      /* recompute */
        seq_t term = L / c;                           /* integer division */

        /* k even contributes positively, k odd negatively. */
        if ((k & 1ULL) == 0)
            result += term;
        else
            result -= term;    /* safe: the sum is known to stay positive */
    }

    return result;
}
