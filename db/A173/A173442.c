/*
 * A173442  Number of divisors d of number n such that sigma(d) does not
 *          divide n.
 * https://oeis.org/A173442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors d of n such that sigma(d) does not
 *            divide n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 9887 (2026-11-01)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

static inline unsigned __int128 sigma_p_term(uint64_t p, unsigned e, uint64_t limit)
{
    /* Compute sigma(p^e) = 1 + p + ... + p^e, stopping if the sum
       would certainly exceed `limit` (used as a hint). */
    unsigned __int128 pow = 1;
    unsigned __int128 sum = 1;
    for (unsigned i = 1; i <= e; ++i) {
        pow *= (unsigned __int128)p;
        sum += pow;
        if (sum > limit) return sum;   /* value > limit, caller will treat it */
    }
    return sum;
}

/* Recursively count divisors d of n whose sigma(d) does not divide n. */
static void dfs_divisors(unsigned idx,
                         unsigned __int128 sigma_so_far,
                         uint64_t n,
                         const uint64_t *primes,
                         const uint64_t *exps,
                         unsigned pcnt,
                         uint64_t *cnt)
{
    /* All remaining choices will only increase sigma, so if we already
       have sigma_so_far > n, every further divisor has sigma > n
       and is counted.  The number of such divisors equals the product
       over remaining primes of (exps[i] + 1). */
    if (sigma_so_far > n) {
        uint64_t ways = 1;
        for (unsigned i = idx; i < pcnt; ++i)
            ways *= (uint64_t)(exps[i] + 1);
        (*cnt) += ways;
        return;
    }

    if (idx == pcnt) {                     /* reached a divisor */
        if (n % (uint64_t)sigma_so_far != 0)
            (*cnt)++;                      /* sigma(d) does not divide n */
        return;
    }

    /* exponent 0 for the current prime */
    dfs_divisors(idx + 1, sigma_so_far, n, primes, exps,
                 pcnt, cnt);

    uint64_t p = primes[idx];
    unsigned maxe = (unsigned)exps[idx];
    for (unsigned e = 1; e <= maxe; ++e) {
        unsigned __int128 term = sigma_p_term(p, e, n);
        unsigned __int128 sigma_new;
        if (term > n || sigma_so_far > (unsigned __int128)n / term)
            sigma_new = (unsigned __int128)n + 1;   /* any value > n */
        else
            sigma_new = sigma_so_far * term;
        dfs_divisors(idx + 1, sigma_new, n, primes, exps,
                     pcnt, cnt);
    }
}

seq_t a(idx_t n)
{
    uint64_t n64 = (uint64_t)n;

    /* factor n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt = 0;
    factor_u64(n64, primes, exps, &pcnt);

    uint64_t count = 0;
    dfs_divisors(0, (unsigned __int128)1, n64, primes, exps,
                 (unsigned)pcnt, &count);

    return (seq_t)count;
}
