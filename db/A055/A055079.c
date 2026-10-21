/*
 * A055079  Smallest number with exactly n nonprime divisors.
 * https://oeis.org/A055079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) is the smallest integer k such that d(k) - ω(k) = n, where
 *            d(k) is the number of divisors of k and ω(k) is the number of
 *            distinct prime divisors of k. The function uses a depth‑first
 *            search over prime exponent patterns, keeping exponents
 *            non‑increasing so that the smallest admissible number...
 * Range:     u8 n <= 10, u16 n <= 34, u32 n <= 34, u64 n <= 70, u128 n <= 190,
 *            big any n
 * Helpers:   cmp_seq_t, pow_uint, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* Helper functions supplied by the build. */
#include "../utils/seq_mul.h"     /* seq_t seq_mul(seq_t a, seq_t b);        */
#include "../utils/pow_uint.h"    /* seq_t pow_uint(uint64_t base, idx_t exp); */
#include "../utils/cmp_seq_t.h"   /* int cmp_seq_t(const void *p1, const void *p2); */

/* List of the first sixteen primes – enough for  n ≤ 10 000. */
static const uint64_t primes[] = {
    2ULL,  3ULL,  5ULL,  7ULL, 11ULL, 13ULL, 17ULL, 19ULL,
    23ULL, 29ULL, 31ULL, 37ULL, 41ULL, 43ULL, 47ULL, 53ULL
};
static const size_t NUM_PRIMES = sizeof(primes) / sizeof(primes[0]);

/* Recursive depth‑first search for the minimal number. */
static void dfs(idx_t target,      /* n */
                idx_t prime_idx,   /* index of current prime      */
                idx_t last_exp,    /* maximum exponent allowed (non‑increasing) */
                idx_t dprod,       /* current divisor product ∏(ei+1)   */
                idx_t depth,       /* number of distinct primes so far   */
                seq_t value,       /* current product of primes^ei      */
                seq_t *best,       /* pointer to best value found so far */
                bool *found)       /* whether a candidate was found    */
{
    if (prime_idx >= NUM_PRIMES)
        return;

    for (idx_t e = 1; e <= last_exp; ++e) {
        idx_t new_dprod = dprod * (e + 1);
        idx_t new_depth = depth + 1;
        idx_t n_val = new_dprod - new_depth;
        if (n_val > target)
            break;                      /* further exponents increase n_val */

        seq_t exp_pow  = pow_uint(primes[prime_idx], e);
        seq_t new_val  = seq_mul(value, exp_pow);

        if (*found && cmp_seq_t(&new_val, best) >= 0)
            break;                      /* any larger e will give an even larger value */

        if (n_val == target) {
            if (!*found || cmp_seq_t(&new_val, best) < 0) {
                *best  = new_val;
                *found = true;
            }
        } else { /* n_val < target */
            dfs(target, prime_idx + 1, e, new_dprod, new_depth, new_val, best, found);
        }
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 1)                 /* the trivial case: 1 has exactly one nonprime divisor (itself) */
        return (seq_t)1;

    seq_t best = 0;
    bool found = false;
    /* Start DFS with an unbounded initial exponent (at most n+1). */
    dfs(n, 0, n, 1, 0, (seq_t)1, &best, &found);
    return best;                 /* a solution always exists for n ≥ 1 */
}
