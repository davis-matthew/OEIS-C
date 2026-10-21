/*
 * A221698  Composite numbers n such that largest digit of all divisors of n
 *          is 2.
 * https://oeis.org/A221698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   factor_u64, has_digit_gt2, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = nth composite number whose every divisor has only decimal digits 0,1,2.
   Computed by incremental search with divisor verification. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#include "../utils/is_prime.h"
#include "../utils/factor_u64.h"
#include "../utils/has_digit_gt2.h"

/* Depth‑first traversal of all divisors given prime factorization.
   Returns false as soon as a divisor with a digit > 2 is found. */
static bool dfs_div(size_t idx, seq_t prod,
                    const uint64_t *pr, const uint64_t *ex, size_t cnt)
{
    if (idx == cnt) {
        /* prod is a complete divisor. */
        return !has_digit_gt2(prod);
    }
    seq_t p = (seq_t)1;
    for (size_t e = 0; e <= ex[idx]; ++e) {
        if (e > 0) p *= (seq_t)pr[idx];
        if (!dfs_div(idx + 1, prod * p, pr, ex, cnt)) return false;
    }
    return true;
}

/* Cache of sequence values (static for non‑monotonic calls). */
static seq_t  *cache      = NULL;
static size_t   cache_cap = 0;
static size_t   cache_len = 0;
static idx_t    last_checked = 3;   /* start checking from 4 */

/* External linkage required. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* unused, present for safety */

    /* Grow cache until it contains at least n entries. */
    while (cache_len < (size_t)n) {
        ++last_checked;

        /* Candidate must have only digits 0,1,2. */
        if (has_digit_gt2((seq_t)last_checked)) continue;

        /* Must be composite. */
        if (is_prime(last_checked)) continue;

        /* Factorization of last_checked. */
        uint64_t primes[64];
        uint64_t exps[64];
        size_t   pcnt = 0;
        factor_u64(last_checked, primes, exps, &pcnt);

        /* Reject if any prime factor itself contains a digit > 2. */
        bool bad = false;
        for (size_t i = 0; i < pcnt && !bad; ++i) {
            if (has_digit_gt2((seq_t)primes[i])) bad = true;
        }
        if (bad) continue;

        /* Check all divisors. */
        if (!dfs_div(0, (seq_t)1, primes, exps, pcnt)) continue;

        /* Store valid composite number. */
        if (cache_len + 1 > cache_cap) {
            size_t newcap = cache_cap ? cache_cap * 2 : 16;
            seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp) { free(cache); cache = NULL; return (seq_t)0; }
            cache = tmp;
            cache_cap = newcap;
        }
        cache[cache_len++] = (seq_t)last_checked;
    }

    return cache[(size_t)n - 1];
}
