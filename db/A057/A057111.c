/*
 * A057111  Numbers n which are a factor of (LPP(n)-1)!, where LPP(n) is the
 *          largest prime power factor of n.
 * https://oeis.org/A057111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A057111: numbers n such that n | (LPP(n)-1)!,
   LPP(n) being the largest prime power factor of n. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Test whether a given integer m satisfies the condition. */
static bool satisfies(idx_t m)
{
    if (m <= 1)
        return false;

    /* factorisation data (enough for numbers in the required range) */
    uint64_t primes[64];
    int        exps[64];
    int        cnt = 0;
    uint64_t tmp = m;

    /* factor 2 */
    if ((tmp & 1ull) == 0) {
        int e = 0;
        while ((tmp & 1ull) == 0) {
            e++;
            tmp >>= 1;
        }
        primes[cnt] = 2;
        exps[cnt]   = e;
        cnt++;
    }

    /* odd factors up to sqrt(tmp) */
    for (uint64_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                e++;
                tmp /= p;
            }
            primes[cnt] = p;
            exps[cnt]   = e;
            cnt++;
        }
    }

    /* remaining prime > sqrt(original m) */
    if (tmp > 1) {
        primes[cnt] = tmp;
        exps[cnt]   = 1;
        cnt++;
    }

    /* compute largest prime power factor (LPP) */
    seq_t lpp = 0;
    for (int i = 0; i < cnt; i++) {
        seq_t pe = 1;
        for (int j = 0; j < exps[i]; j++)
            pe *= (seq_t)primes[i];
        if (pe > lpp)
            lpp = pe;
    }
    if (lpp == 0)
        return false;                 /* shouldn't happen for m > 1 */

    seq_t bound = lpp - 1;            /* (LPP-1) */

    /* check that every prime factor of m divides (bound)! */
    for (int i = 0; i < cnt; i++) {
        uint64_t q = primes[i];
        seq_t exp_fact = 0;
        seq_t power = q;
        while (power <= bound) {
            exp_fact += bound / power;
            if (power > bound / q)
                break;
            power *= q;
        }
        if (exp_fact < (seq_t)exps[i])
            return false;
    }

    return true;
}

/* Cache for previously computed sequence values. */
static seq_t *   cache      = NULL;
static size_t    cache_len  = 0;
static size_t    cache_cap  = 0;
static idx_t    last_checked = 0;

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* undefined for offset 1 */

    /* generate terms until we have n of them */
    while (cache_len < n) {
        last_checked++;
        if (satisfies(last_checked)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 4;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)   /* OOM: return 0 (unlikely in tests) */
                    return 0;
                cache      = tmp;
                cache_cap  = new_cap;
            }
            cache[cache_len++] = (seq_t)last_checked;
        }
    }

    return cache[n - 1];
}
