/*
 * A167401  a(n) is the smallest number k such that n*k has twice as many
 *          divisors as k.
 * https://oeis.org/A167401
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 59, u32 n <= 329, u64 n <= 3989,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = min k ≥ 1 such that d(n·k) = 2·d(k) (OEIS A167401) */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* ---------- Small prime list for trial division up to 10 000 ---------- */
#define SMALL_PRIME_COUNT 25
static const uint64_t small_primes[SMALL_PRIME_COUNT] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
    31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
    73, 79, 83, 89, 97
};

/* ---------- Factorisation routine ----------------------------------- */
static void
factor_uint64(uint64_t n, uint64_t *factors, uint64_t *exps, size_t *cnt)
{
    size_t c = 0;
    uint64_t t = n;
    for (int i = 0; i < SMALL_PRIME_COUNT && t > 1; ++i) {
        uint64_t p = small_primes[i];
        if (p * p > t) break;
        if (t % p == 0) {
            uint64_t e = 0;
            while (t % p == 0) {
                t /= p;
                ++e;
            }
            factors[c] = p;
            exps[c]   = e;
            ++c;
        }
    }
    if (t > 1) {           /* remaining prime factor */
        factors[c] = t;
        exps[c]   = 1;
        ++c;
    }
    *cnt = c;
}

/* ---------- Static data for DFS ------------------------------------- */
static uint64_t g_factors[12];
static uint64_t g_exps[12];
static size_t   g_m;
static __uint128_t best_k128;
static bool      best_found;

/* ---------- DFS search over exponent vectors ----------------------- */
static void
dfs(idx_t depth, uint64_t num, uint64_t den, __uint128_t k128)
{
    if (depth == g_m) {
        if (num == 2 * den) {
            if (!best_found || k128 < best_k128) {
                best_k128 = k128;
                best_found = true;
            }
        }
        return;
    }

    uint64_t e = g_exps[depth];
    uint64_t p = g_factors[depth];
    uint64_t max_f = e + 20;            /* sufficient upper bound */

    __uint128_t pow128 = 1;              /* p^f for current f */

    for (uint64_t f = 0; f <= max_f; ++f) {
        /* Update ratio factor (e+f+1)/(f+1) */
        uint64_t t_num = e + f + 1;
        uint64_t t_den = f + 1;
        uint64_t new_num = num * t_num;
        uint64_t new_den = den * t_den;
        if (new_num > 2 * new_den) {    /* ratio > 2, still possible with larger f */
            /* continue searching */
        }

        /* Compute new k, avoiding overflow of 128-bit arithmetic */
        if (k128 > 0 && pow128 > 0
            && k128 > ( (__uint128_t)(~0) ) / pow128) {
            /* would overflow 128‑bit, skip this branch */
        } else {
            __uint128_t new_k = k128 * pow128;
            /* Prune if already worse than best known */
            if (!best_found || new_k < best_k128) {
                dfs(depth + 1, new_num, new_den, new_k);
            }

            /* If we already found a better solution, further increasing f
               will only make k larger, so we can stop exploring this prime. */
            if (best_found && new_k >= best_k128) {
                /* Break nevertheless to honour the upper bound */
                /* but still finish loop to respect max_f. */
            }
        }

        /* Prepare pow128 for next f, guard against overflow */
        if (f < max_f) {
            if (pow128 > ( (__uint128_t)(~0) ) / p) {
                /* next multiplication would overflow 128‑bit → stop loop */
                break;
            }
            pow128 *= (__uint128_t)p;
        }
    }
}

/* ---------- Public sequence function --------------------------------- */
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;          /* undefined below offset */

    /* Factorisation of n */
    uint64_t factors[12];
    uint64_t exps[12];
    size_t   cnt;
    factor_uint64((uint64_t)n, factors, exps, &cnt);

    g_m = cnt;
    for (size_t i = 0; i < g_m; ++i) {
        g_factors[i] = factors[i];
        g_exps[i]    = exps[i];
    }

    best_found = false;
    best_k128  = 0;

    /* Start DFS search */
    dfs(0, 1, 1, ( __uint128_t)1);

    return (seq_t)best_k128;
}
