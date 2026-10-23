/*
 * A126080  a(n) = number of positive integers < n that are coprime to exactly
 *          one prime divisor of n.
 * https://oeis.org/A126080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{p|n} ( ⌊(n‑1)/(rad(n)/p)⌋ – ⌊(n‑1)/rad(n)⌋ ) where
 *            rad(n) is the product of distinct prime divisors of n. a(1) = 0
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    /* Factor n into primes (distinct).  factor_u64 returns
       primes in the array `primes` and their exponents in `exps`.  */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    /* rad(n) = product of the distinct primes. */
    idx_t rad = 1;
    for (size_t i = 0; i < cnt; ++i)
        rad *= primes[i];

    /* S = ⌊(n‑1)/rad⌋. */
    idx_t S = (n - 1) / rad;

    seq_t ans = 0;
    for (size_t i = 0; i < cnt; ++i) {
        idx_t M = rad / primes[i];           /* product of all primes except primes[i] */
        idx_t term = (n - 1) / M - S;        /* ⌊(n‑1)/M⌋ – S   */
        ans += (seq_t)term;
    }
    return ans;
}
