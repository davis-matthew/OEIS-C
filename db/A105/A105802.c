/*
 * A105802  Smallest m such that the m-th Fibonacci number has exactly n
 *          divisors that are also Fibonacci numbers.
 * https://oeis.org/A105802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 31, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A105802 – smallest index m such that the
 * m‑th Fibonacci number has exactly n distinct
 * Fibonacci divisors.
 * The number of distinct Fibonacci divisors of F_m equals
 * tau(m) minus 1 if m is even (because F_1 = F_2 = 1).
 * Therefore we have to find the smallest m satisfying
 *     tau(m) - (m%2==0 ? 1 : 0) == n
 * Equivalently:
 *  * odd m  ->  tau(m) = n
 *  * even m ->  tau(m) = n+1
 * The minimal number with a given divisor count is
 * obtained by a decreasing sequence of prime exponents.
 * This implementation searches for minimal odd
 * and minimal even numbers separately and returns the
 * smaller of the two.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow_uint.h"   /* seq_t pow_uint(seq_t base, unsigned long long exp); */

/* Prime list – enough for divisor counts up to 10000 */
static const uint32_t primes[] = {
    2u, 3u, 5u, 7u, 11u, 13u, 17u, 19u,
    23u, 29u, 31u, 37u, 41u, 43u, 47u, 53u
};
#define NUM_PRIMES (sizeof(primes)/sizeof(primes[0]))

/* Cache for the first 10000 terms.
 * 0 means not yet computed (except for n=0 which is unused). */
static seq_t cache[10001] = { 0 };

/* Recursive search for minimal number with given divisor count.
 * idx  – index into primes array (skip 2 when odd search).
 * last – maximum exponent allowed (non‑increasing property).
 * remain – remaining product of (exp+1).
 * cur    – current constructed number.
 * best   – pointer to current best (minimal) value. */
static void dfs_minimal(uint32_t idx, uint32_t last, uint32_t remain,
                        seq_t cur, seq_t *best)
{
    if (remain == 1u) {
        /* All divisors accounted for */
        if (*best == 0 || cur < *best) *best = cur;
        return;
    }
    if (idx >= NUM_PRIMES) return;          /* out of primes */

    uint32_t prime = primes[idx];
    /* Try all exponents e such that (e+1) divides remain and e <= last */
    for (uint32_t e = 1; e <= last; ++e) {
        uint32_t d = e + 1u;
        if (remain % d != 0u) continue;
        /* compute new current value */
        seq_t powp = pow_uint((seq_t)prime, (unsigned long long)e);
        seq_t new_cur = cur * powp;
        if (*best != 0 && new_cur > *best) continue;  /* prune */
        dfs_minimal(idx + 1, e, remain / d, new_cur, best);
    }
}

/* Minimal number with divisor count K.
 * If require_even = true, the exponent of 2 must be >=1.
 * If require_even = false, 2 must not appear (i.e. odd number). */
static seq_t minimal_with_divisors(uint32_t K, bool require_even)
{
    seq_t best = 0;

    if (require_even) {
        /* Pick exponent for 2 first */
        for (uint32_t e = 1; e <= K; ++e) {
            uint32_t d = e + 1u;
            if (K % d != 0u) continue;
            seq_t val2 = pow_uint((seq_t)2u, (unsigned long long)e);
            seq_t cur = val2;
            /* search with remaining divisor product */
            dfs_minimal(1, e, K / d, cur, &best);
        }
    } else {
        /* No factor 2 – start from prime 3 */
        dfs_minimal(1, K, K, (seq_t)1, &best);
    }
    return best;
}

seq_t a(idx_t n)
{
    if (n <= 1u) return (seq_t)1;   /* a(1)=1 */
    if (n <= 10000u && cache[n] != 0) return cache[n];

    /* Search odd candidate (tau = n) */
    seq_t odd_candidate = minimal_with_divisors((uint32_t)n, false);
    /* Search even candidate (tau = n+1) */
    seq_t even_candidate = minimal_with_divisors((uint32_t)(n + 1u), true);

    seq_t result;
    if (odd_candidate == 0) result = even_candidate;
    else if (even_candidate == 0) result = odd_candidate;
    else result = odd_candidate < even_candidate ? odd_candidate : even_candidate;

    if (n <= 10000u) cache[n] = result;
    return result;
}
