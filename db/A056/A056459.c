/*
 * A056459  Number of primitive (aperiodic) palindromes using a maximum of
 *          three different symbols.
 * https://oeis.org/A056459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} μ(d) * 3^{ceil((n/d)/2)}
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   power_of_three
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here μ(d) is the Möbius function.  The function computes the number
   of primitive (aperiodic) palindromes of length n that use at most
   three different symbols. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/power_of_three.h"

/* Recursive enumeration of all square‑free divisors of n.  For each
   divisor d the Möbius value is (-1)^{ω(d)} where ω(d) is the number
   of prime factors of d.  The sums of positive and negative terms are
   accumulated separately to avoid intermediate over‑/underflows. */
static void dfs(uint64_t idx,
                idx_t d,
                int sign,
                idx_t n,
                const int *primes,
                size_t pcnt,
                seq_t *pos,
                seq_t *neg)
{
    if (idx == pcnt) {
        idx_t t = n / d;
        idx_t k = (t + 1) >> 1;                 /* ceil(t/2) */
        seq_t val = power_of_three(k);
        if (sign > 0)
            *pos += val;
        else
            *neg += val;
        return;
    }

    /* Branch where the current prime is not used (exponent 0) */
    dfs(idx + 1, d, sign, n, primes, pcnt, pos, neg);

    /* Branch where the current prime is used exactly once (exponent 1) */
    if (d <= n / (idx_t)primes[idx]) {
        dfs(idx + 1, d * (idx_t)primes[idx], -sign, n,
            primes, pcnt, pos, neg);
    }
}

seq_t a(idx_t n)
{
    /* Factorisation of n by trial division (n <= 2^64-1) */
    int primes[64];
    size_t pcnt = 0;
    idx_t tmp = n;

    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            primes[pcnt++] = (int)p;
            while (tmp % p == 0)
                tmp /= p;
        }
    }
    if (tmp > 1)
        primes[pcnt++] = (int)tmp;

    seq_t pos = 0, neg = 0;
    dfs(0, 1, 1, n, primes, pcnt, &pos, &neg);

    return pos - neg;
}
