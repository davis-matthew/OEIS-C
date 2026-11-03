/*
 * A173440  Number of divisors d of number n such that sigma(d) does not
 *          divide sigma(n).
 * https://oeis.org/A173440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1648, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 121 terms through n = 9678 (2026-11-01)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A173440
    a(n) = number of divisors d of n such that sigma(d) does not divide sigma(n).
    We factor n, enumerate all its divisors recursively, and test the
    sigma‑divisibility condition.  No dynamic allocation is required.
    The algorithm is O(number of divisors of n) and works for all n≥1.
*/

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

/* Recursive enumeration of divisors.
   cur_sigma   : sigma of the divisor constructed so far
   idx         : which prime factor we are processing
   sigma_np    : sigma(n)
   primes, exps: prime factorisation of n
   cnt         : number of distinct prime factors
   res         : pointer to the running count of bad divisors
*/
static void dfs(size_t idx, seq_t cur_sigma, seq_t sigma_np,
                const uint64_t *primes, const uint64_t *exps,
                size_t cnt, seq_t *res)
{
    if (idx == cnt) {
        if (sigma_np % cur_sigma != 0) (*res)++;
        return;
    }

    /* sigma(p^e) is computed iteratively: 1, p+1, p²+p+1, ... */
    seq_t sigma_factor = 1;                     /* sigma(p^0) */
    for (uint64_t e = 0; e <= exps[idx]; ++e) {
        dfs(idx + 1, cur_sigma * sigma_factor, sigma_np,
            primes, exps, cnt, res);
        sigma_factor = sigma_factor * primes[idx] + 1;   /* next sigma(p^e) */
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* offset starts at 1 */

    /* Factorise n.  The helper writes primes and exponents into the
       provided arrays and stores the count in prime_cnt */
    uint64_t primes[16];
    uint64_t exps[16];
    size_t prime_cnt;
    factor_u64((uint64_t)n, primes, exps, &prime_cnt);

    /* sigma(n) */
    seq_t sigma_np = sigma_of(n);

    /* Count divisors d with sigma(d) not dividing sigma(n) */
    seq_t result = 0;
    dfs(0, 1, sigma_np, primes, exps, prime_cnt, &result);

    return result;
}
