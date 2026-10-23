/*
 * A189712  Numbers m such that for each prime p that divides m, there is a
 *          k(p) such that k(p) == 1 (mod p), k(p) divides m evenly, and m
 *          divides k(p)!/2 evenly.
 * https://oeis.org/A189712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth integer m (m ≥ 2) such that for each prime
 *            divisor p of m there exists a divisor k of m with k ≡ 1 (mod p)
 *            and m | k! / 2. The sequence starts with a(1) = 30. The
 *            algorithm searches m upwards, testing the condition by
 *            enumerating all divisors of m and checking the factorial
 *            condit...
 * Range:     u8 n <= 13, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   factor_u64, factorial_condition
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/factorial_condition.h"

static seq_t *cache = NULL;          /* cached terms */
static size_t cache_len = 0;         /* number of cached terms */
static uint64_t last_checked = 0;    /* last m examined */

static void gen_divisors(size_t idx,
                         uint64_t cur,
                         const uint64_t *pr,
                         const uint64_t *exps,
                         size_t pcnt,
                         uint64_t *divs,
                         size_t *dcnt)
{
    if (idx == pcnt) {
        divs[(*dcnt)++] = cur;
        return;
    }
    uint64_t p = pr[idx];
    uint64_t val = cur;
    for (size_t e = 0; e <= exps[idx]; ++e) {
        gen_divisors(idx + 1, val, pr, exps, pcnt, divs, dcnt);
        val *= p;
    }
}

static bool satisfies_m(uint64_t m)
{
    if (m < 2) return false;
    /* factorize m */
    uint64_t primes[64], exps[64];
    size_t pcnt;
    factor_u64(m, primes, exps, &pcnt);

    /* generate all divisors of m */
    uint64_t divs[1024];
    size_t dcnt = 0;
    gen_divisors(0, 1, primes, exps, pcnt, divs, &dcnt);

    /* for each prime factor p, find a suitable divisor k */
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = primes[i];
        bool ok = false;
        for (size_t j = 0; j < dcnt; ++j) {
            uint64_t d = divs[j];
            if (d % p == 1) {
                if (factorial_condition(d, m, primes, exps, pcnt)) {
                    ok = true;
                    break;
                }
            }
        }
        if (!ok) return false;
    }
    return true;
}

static void ensure_cache(idx_t n)
{
    if (cache_len >= n) return;
    /* grow cache */
    size_t new_cap = cache_len > 0 ? cache_len * 2 : 1;
    while (new_cap < n) new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    /* fill cache */
    while (cache_len < n) {
        ++last_checked;
        if (satisfies_m(last_checked)) {
            cache[cache_len++] = (seq_t)last_checked;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but harmless */
    ensure_cache(n);
    return cache[n - 1];
}
