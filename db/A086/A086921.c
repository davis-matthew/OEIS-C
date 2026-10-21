/*
 * A086921  Least number with at least n divisors that are at most its square
 *          root.
 * https://oeis.org/A086921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = least m with at least 2*n-1 divisors (equivalently, m has
 *            at least n unordered factor pairs p*q with p ≤ q)
 * Range:     u8 n <= 6, u16 n <= 27, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

#define PRIME_COUNT 15
static const uint64_t primes[PRIME_COUNT] = {
    2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL,
    23ULL, 29ULL, 31ULL, 37ULL, 41ULL, 43ULL, 47ULL
};

/* Cache results for divisor thresholds up to 20000 (needed for n ≤ 10000) */
static seq_t memo[20001] = {0};

/* Recursive search for smallest m with divisor count ≥ target.
   pos      : current prime index
   last_exp : maximum exponent allowed (non‑increasing sequence)
   cur_val  : product of primes so far
   cur_div  : current divisor count (product of (exp+1))
   target   : desired divisor count
   best     : pointer to the current best solution           */
static void dfs(int pos, int last_exp, seq_t cur_val, idx_t cur_div,
                idx_t target, seq_t *best)
{
    if (cur_div >= target) {
        if (*best == 0 || cur_val < *best) *best = cur_val;
        return;
    }
    if (pos >= PRIME_COUNT) return;
    seq_t val = cur_val;
    for (int exp = 1; exp <= last_exp; ++exp) {
        val *= (seq_t)primes[pos];
        if (*best != 0 && val > *best) break;   /* pruning */
        idx_t new_div = cur_div * (exp + 1);
        if (new_div >= target) {
            if (*best == 0 || val < *best) *best = val;
        } else {
            dfs(pos + 1, exp, val, new_div, target, best);
        }
    }
}

/* Minimal value with at least 'target' divisors */
static seq_t min_with_at_least_div(idx_t target)
{
    if (target <= 1) return 1;               /* 1 has one divisor */
    seq_t res = memo[target];
    if (res != 0) return res;
    seq_t best = 0;
    dfs(0, 60, 1, 1, target, &best);
    memo[target] = best;
    return best;
}

/* Main interface required by the build */
seq_t a(idx_t n)
{
    /* a(n) = minimal m with at least 2*n-1 divisors */
    idx_t target = 2 * n - 1;
    return min_with_at_least_div(target);
}
