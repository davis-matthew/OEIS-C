/*
 * A152443  a(n) = the largest proper divisor of n that is coprime to d(n),
 *          where d(n) = the number of divisors of n.
 * https://oeis.org/A152443
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = the largest proper divisor of n that is coprime to d(n)
 *            where d(n) is the number of divisors of n
 * Range:     u8 n <= 513, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/factor_u64.h"

/* Recursive helper to explore all divisors of n. */
static void
dfs_div(size_t idx,
        const uint64_t *pr, const uint64_t *ex,
        size_t cnt,
        uint64_t cur,
        uint64_t n,
        seq_t d_n,
        seq_t *best)
{
    if (idx == cnt) {
        if (cur != n) {
            seq_t cur_s = (seq_t)cur;
            if (gcd(cur_s, d_n) == 1 && cur_s > *best) {
                *best = cur_s;
            }
        }
        return;
    }

    uint64_t power = 1;
    for (size_t e = 0; e <= (size_t)ex[idx]; ++e) {
        uint64_t new_cur = cur * power;
        dfs_div(idx + 1, pr, ex, cnt, new_cur, n, d_n, best);
        if (e < (size_t)ex[idx]) {
            /* next power of prime */
            power *= pr[idx];
        }
    }
}

seq_t a(idx_t n)
{
    /* Cast n to 64‑bit unsigned for internal use. */
    uint64_t nn = (uint64_t)n;
    /* Compute prime factorisation of n. */
    uint64_t pr[64];          /* primes */
    uint64_t ex[64];          /* exponents */
    size_t   cnt = 0;
    factor_u64(nn, pr, ex, &cnt);

    /* Compute d(n) = product (ex_i + 1). */
    seq_t d_n = 1;
    for (size_t i = 0; i < cnt; ++i) {
        d_n *= (seq_t)(ex[i] + 1);
    }

    /* Search all divisors of n (except n itself) and keep the largest
       that is coprime to d_n.  1 is always a candidate. */
    seq_t best = 1;
    dfs_div(0, pr, ex, cnt, 1ULL, nn, d_n, &best);

    return best;
}
