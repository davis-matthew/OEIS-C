/*
 * A096529  Numbers whose divisors can be permuted so that all sums of triple
 *          adjacent divisors are primes.
 * https://oeis.org/A096529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Validated: standard, pass, 74 terms through n = 74 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A096529 – numbers whose divisors can be permuted so that all sums of
   three consecutive divisors are prime.  The function a(idx_t n)
   returns the n‑th term (offset 1).  The sequence is built lazily
   and cached.  The algorithm factorises each candidate number,
   generates all of its divisors, and searches for a permutation
   that satisfies the prime‑sum condition. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/is_prime.h"
#include "../utils/mem.h"

/* ---------- Trial division factorisation ----------------------------------- */
static void _factor_u64(uint64_t n, uint64_t *pr, uint64_t *exp, size_t *cnt)
{
    *cnt = 0;
    if (n < 2) return;
    /* factor 2 separately */
    if ((n & 1ULL) == 0) {
        pr[*cnt] = 2ULL;
        exp[*cnt] = 0;
        while ((n & 1ULL) == 0) {
            n >>= 1;
            exp[*cnt] += 1;
        }
        (*cnt)++;
    }
    /* odd primes */
    for (uint64_t d = 3ULL; d * d <= n; d += 2ULL) {
        if (n % d == 0) {
            pr[*cnt] = d;
            exp[*cnt] = 0;
            while (n % d == 0) {
                n /= d;
                exp[*cnt] += 1;
            }
            (*cnt)++;
        }
    }
    if (n > 1) {
        pr[*cnt] = n;
        exp[*cnt] = 1;
        (*cnt)++;
    }
}

/* ---------- helper: generate all divisors ---------- */
static void gen_divs_rec(const uint64_t *pr, const uint64_t *exp,
                         uint64_t pf_cnt, uint64_t idx, uint64_t cur,
                         uint64_t *dv, size_t *d)
{
    if (idx == pf_cnt) {
        dv[(*d)++] = cur;
        return;
    }
    uint64_t val = cur;
    for (uint64_t e = 0; e <= exp[idx]; ++e) {
        gen_divs_rec(pr, exp, pf_cnt, idx + 1, val, dv, d);
        val *= pr[idx];
    }
}

static void gen_divs(uint64_t n, const uint64_t *pr, const uint64_t *exp,
                     uint64_t pf_cnt, uint64_t *dv, size_t *d)
{
    *d = 0;
    gen_divs_rec(pr, exp, pf_cnt, 0, 1, dv, d);
}

/* ---------- helper: search a valid permutation ---------- */
static bool dfs_perm(size_t depth, size_t last1, size_t last2,
                     bool *used, const uint64_t *dv, size_t d)
{
    if (depth == d)               /* all divisors placed */
        return true;
    for (size_t j = 0; j < d; ++j) {
        if (used[j])
            continue;
        uint64_t sum = dv[last1] + dv[last2] + dv[j];
        if (!is_prime((idx_t)sum))
            continue;
        used[j] = true;
        if (dfs_perm(depth + 1, last2, j, used, dv, d))
            return true;          /* success down this branch */
        used[j] = false;          /* backtrack */
    }
    return false;                 /* no completion possible */
}

/* ---------- test whether a number has the property ---------- */
static bool has_property(uint64_t n)
{
    uint64_t pr[32];
    uint64_t exp[32];
    size_t pf_cnt = 0;

    _factor_u64(n, pr, exp, &pf_cnt);

    /* generate all divisors – at most ~2000 for 64‑bit numbers */
    static uint64_t divs[4096];
    size_t d;
    gen_divs(n, pr, exp, pf_cnt, divs, &d);

    if (d < 3)                     /* cannot form a triple */
        return false;

    if (d == 3) {                  /* only one triple – sum of all */
        uint64_t sum = divs[0] + divs[1] + divs[2];
        return is_prime((idx_t)sum);
    }

    bool used[4096];
    for (size_t i = 0; i < d; ++i) {
        for (size_t j = 0; j < d; ++j) {
            if (i == j)
                continue;
            memset(used, 0, d * sizeof(bool));
            used[i] = used[j] = true;
            if (dfs_perm(2, i, j, used, divs, d))
                return true;
        }
    }
    return false;                   /* no permutation works */
}

/* ------------------------------------------------------------ */
/* public sequence routine                                          */
static seq_t *seq_cache = NULL;   /* cached terms                */
static size_t cache_len = 0;      /* number of cached terms      */
static uint64_t checked = 0;      /* last integer considered     */

seq_t a(idx_t n)
{
    if (n == 0)                    /* offset 1 – unused          */
        return (seq_t)0;

    while (cache_len < (size_t)n) {
        ++checked;
        if (has_property(checked)) {
            seq_t *tmp = mem_resize(seq_t, seq_cache, (cache_len + 1));
            if (!tmp)
                exit(1);          /* allocation failure */
            seq_cache = tmp;
            seq_cache[cache_len++] = (seq_t)checked;
        }
    }
    return seq_cache[n - 1];
}
