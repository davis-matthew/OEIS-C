/*
 * A236328  a(n) = sigma(n,1) + sigma(n,2) + ... + sigma(n,n).
 * https://oeis.org/A236328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + Sum_{d|n, d>1} d*(d^n - 1)/(d - 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/factor_u64.h"

/* Recursively enumerate all divisors > 1 of n.  
   cur holds the product of the chosen prime powers so far. */
static void traverse_divisors(const uint64_t *primes,
                              const uint64_t *exps,
                              size_t cnt,
                              size_t pos,
                              seq_t cur,
                              seq_t *sum,
                              idx_t n)
{
    if (pos == cnt) {
        if (cur == (seq_t)1) return;           /* skip divisor 1 */
        seq_t d   = cur;
        seq_t dn  = big_pow(d, (seq_t)n);      /* d^n */
        dn -= (seq_t)1;
        dn *= d;
        dn /= (d - (seq_t)1);                  /* d*(d^n-1)/(d-1) */
        *sum += dn;
        return;
    }
    /* Iterate over exponent for the current prime factor */
    seq_t cur_next = cur;
    for (uint64_t e = 0; e <= exps[pos]; ++e) {
        traverse_divisors(primes, exps, cnt, pos + 1, cur_next, sum, n);
        cur_next *= (seq_t)primes[pos];
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n = 0 */
    if (n == 1) return (seq_t)1;   /* only divisor is 1 itself */

    /* start with the trivial divisor 1 term, which is n */
    seq_t total = (seq_t)n;

    /* factor n (uint64_t fits all values needed for n ≤ 10 000) */
    uint64_t primes[64], exps[64];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* enumerate all non‑trivial divisors and add their contributions */
    traverse_divisors(primes, exps, cnt, 0, (seq_t)1, &total, n);

    return total;
}
