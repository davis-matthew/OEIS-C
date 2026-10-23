/*
 * A187941  Least number with exactly n even divisors.
 * https://oeis.org/A187941
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Least number with exactly n even divisors.
   For N = 2^k · p1^e1 · … · pr^er the number of even divisors is
   k · (e1+1) · … · (er+1).  Therefore the least N with n even divisors
   is 2 times the least integer with exactly n (ordinary) divisors. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>   /* for size_t */

/* Primes large enough for n ≤ 10000.  16 primes are safe. */
static const uint64_t FIRST_PRIMES[] = {
    2ull, 3ull, 5ull, 7ull, 11ull, 13ull, 17ull, 19ull,
    23ull, 29ull, 31ull, 37ull, 41ull, 43ull, 47ull, 53ull
};
static const size_t NUM_PRIMES = sizeof(FIRST_PRIMES) / sizeof(FIRST_PRIMES[0]);

/* Depth‑first search for the smallest number whose divisor count equals
   rem.  The search keeps exponents non‑increasing to avoid duplicates. */
static void dfs(const uint64_t *pr, size_t pr_cnt, size_t idx, int max_exp,
                seq_t cur, idx_t rem, seq_t *best)
{
    if (rem == (idx_t)1) {
        /* All required factors used.  cur is a candidate. */
        if (*best == (seq_t)0 || cur < *best)
            *best = cur;
        return;
    }
    if (idx >= pr_cnt)
        return;                    /* no more primes available */

    seq_t curPow = cur;
    for (int e = 1; e <= max_exp; ++e) {
        curPow *= (seq_t)pr[idx];      /* multiply by prime^e */
        if (rem % (idx_t)(e + 1) != 0) /* e+1 must divide rem */
            continue;
        if (*best != (seq_t)0 && curPow > *best) /* pruning */
            break;
        dfs(pr, pr_cnt, idx + 1, e,
            curPow, rem / (idx_t)(e + 1), best);
    }
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;           /* a(0) = 1 */

    seq_t best = (seq_t)0;
    /* a(n) = 2 * minimal integer with exactly n divisors */
    dfs(FIRST_PRIMES, NUM_PRIMES, 0, 64, (seq_t)1, n, &best);
    return best * (seq_t)2;
}
