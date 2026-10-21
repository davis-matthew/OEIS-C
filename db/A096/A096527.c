/*
 * A096527  Number of permutations of divisors of n such that all sums of
 *          triple adjacent divisors are primes.
 * https://oeis.org/A096527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the number of permutations of the divisors of n * such
 *            that for every triple of consecutive divisors in the permutation
 *            * the sum of the three divisors is prime. The function
 *            implements a * search algorithm with strong pruning: it
 *            enumerates all ordered * pairs of first two divisors and th...
 * Range:     u8 n <= 59, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   enum_divs, factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/enum_divs.h"
#include "../utils/factor.h"

/* --------------------------------------------------------------- */
/*  Deterministic Miller–Rabin test for 64‑bit integers            */
/* --------------------------------------------------------------- */
static inline bool is_prime64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        if (n == small[i]) return true;
        if (n % small[i] == 0) return false;
    }
    uint64_t d = n-1, s = 0;
    while ((d & 1ULL) == 0ULL) { d >>= 1; ++s; }

    static const uint64_t bases[] =
     {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL};

    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        __uint128_t cur = a, exp = d, mod = n;
        __uint128_t res = 1;
        while (exp) {
            if (exp & 1ULL) res = (res * cur) % mod;
            cur = (cur * cur) % mod;
            exp >>= 1;
        }
        if (res == 1 || res == n-1) continue;
        bool cont = false;
        for (uint64_t r = 1; r < s; ++r) {
            res = (res * res) % mod;
            if (res == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* --------------------------------------------------------------- */
/*  Simple trial division factorisation and divisor enumeration    */

/* --------------------------------------------------------------- */
/*  Depth‑first search over permutations                          */
/* --------------------------------------------------------------- */
typedef struct { seq_t *cnt; uint64_t *divs; uint32_t d; uint64_t full; } dfs_ctx_t;

static void dfs(uint64_t mask, int pos,
                uint64_t prev1, uint64_t prev2,
                dfs_ctx_t *ctx)
{
    if (pos == ctx->d) { ++(*ctx->cnt); return; }
    uint64_t rem = ctx->full ^ mask;
    while (rem) {
        uint64_t lsb = rem & -rem;
        int i = __builtin_ctzll(rem);
        rem -= lsb;
        uint64_t val = ctx->divs[i];
        if (pos >= 2) {
            uint64_t sum = prev1 + prev2 + val;
            if (!is_prime64(sum)) continue;
        }
        dfs(mask | lsb, pos+1, prev2, val, ctx);
    }
}

/* --------------------------------------------------------------- */
/*  Main sequence function                                        */
/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* factorise n */
    uint64_t pr[64], exp[64];
    size_t pcnt;
    factor((uint64_t)n, pr, exp, &pcnt);

    /* enumerate divisors */
    uint64_t divs[256];
    size_t dcnt = 0;
    enum_divs(0, 1, pr, exp, pcnt, divs, &dcnt);

    uint32_t d = (uint32_t)dcnt;
    if (d < 3) return (seq_t)0;                /* need at least 3 divisors */

    uint64_t fullMask = (d == 64) ? ~0ULL : ((1ULL << d) - 1);

    seq_t count = 0;
    dfs_ctx_t ctx = { &count, divs, d, fullMask };

    /* start permutations with all ordered pairs of first two divisors */
    for (uint32_t i = 0; i < d; ++i) {
        uint64_t mask_i = 1ULL << i;
        for (uint32_t j = 0; j < d; ++j) {
            if (i == j) continue;
            uint64_t mask_j = 1ULL << j;
            uint64_t mask = mask_i | mask_j;
            dfs(mask, 2, divs[i], divs[j], &ctx);
        }
    }
    return count;
}
