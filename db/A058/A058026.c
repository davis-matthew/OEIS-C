/*
 * A058026  Number of positive integers, k, where k <= n and gcd(k,n) =
 *          gcd(k+1,n) = 1.
 * https://oeis.org/A058026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = n * ∏_{p|n} (1 - 2/p) (OEIS A058026)
 * Range:     u8 n <= 262, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factorize
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the multiplicative property: a(p^e) = p^(e-1)*(p-2)          */
# include <stdint.h>
# include "../utils/factorize.h"

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for 0 – return 0 */
        return 0;
    if (n == 1)
        return 1;        /* a(1) = 1 */

    /* factorize returns the distinct prime factors of n */
    uint64_t primes[64];
    int      exps[64];
    size_t   cnt = factorize(n, primes, exps);

    seq_t res = (seq_t)n;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        /* Each distinct prime contributes a factor (p-2)/p   */
        res = (res / p) * (p - 2);
        if (res == 0)        /* early exit – product cannot be negative */
            break;
    }
    return res;
}
