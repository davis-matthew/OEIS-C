/*
 * A225184  Primes p with a primitive root that divides p+1.
 * https://oeis.org/A225184
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 4022, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _smallest_prime_factor, append_candidate, mod_pow, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute primes p such that a primitive root modulo p divides p+1 */
/* The function iterates over primes, factors p-1 and p+1, generates all
 * divisors of p+1 (excluding 1), and checks whether any of them is a
 * primitive root modulo p.  The resulting primes are cached so that
 * subsequent calls are efficient.  The implementation uses the helper
 * functions in the utils directory. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/append_candidate.h"
#include "../utils/mod_pow.h"
#include "../utils/next_prime.h"
#include "../utils/_smallest_prime_factor.h"

/* Helper: test whether g is a primitive root modulo prime p.
 * The prime divisors of p‑1 are supplied in the array pf (size cnt). */
static bool is_primitive_root(uint64_t g, uint64_t p,
                              const uint64_t *pf, size_t cnt)
{
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t q = pf[i];
        seq_t res = mod_pow((seq_t)g, (p - 1) / q, (seq_t)p);
        if (res == 1)
            return false;
    }
    return true;
}

/* Helper: generate all divisors of a number from its prime factors.
 * Factors and their exponents are in pf and exp.  The divisors are
 * stored in out (cnt is updated).  limit is > any divisor to consider. */
static void gen_divisors(const uint64_t *pf, const uint8_t *exp,
                         size_t nf, uint64_t cur, uint64_t *out,
                         size_t *cnt, uint64_t limit)
{
    if (cur > 1 && cur < limit)
        out[(*cnt)++] = cur;
    if (nf == 0)
        return;

    uint64_t prime = pf[0];
    uint8_t emax = exp[0];
    uint64_t prod = 1;
    for (uint8_t e = 0; e <= emax; ++e) {
        gen_divisors(pf + 1, exp + 1, nf - 1,
                     cur * prod, out, cnt, limit);
        prod *= prime;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Static cache for computed values */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;

    /* Last processed prime (next_prime(start) returns > start). */
    static uint64_t last_prime = 1;   /* start before the first prime */

    /* Ensure at least n terms are cached. */
    while (cache_len < (size_t)n) {
        /* Get next prime. */
        last_prime = next_prime(last_prime);
        uint64_t p = last_prime;

        /* Handle prime 2 specially – its only primitive root 1 divides 3. */
        if (p == 2) {
            append_candidate(&cache, &cache_len, &cache_cap, (seq_t)p);
            continue;
        }

        /* Factor p-1: collect distinct prime divisors. */
        uint64_t pf1[32];
        size_t cnt1 = 0;
        uint64_t tmp = p - 1;
        while (tmp > 1) {
            uint64_t f = _smallest_prime_factor(tmp);
            pf1[cnt1++] = f;
            while (tmp % f == 0)
                tmp /= f;
        }

        /* Factor p+1: collect prime factors and their exponents. */
        uint64_t pf2[32];
        uint8_t  exp2[32];
        size_t cnt2 = 0;
        tmp = p + 1;
        while (tmp > 1) {
            uint64_t f = _smallest_prime_factor(tmp);
            uint8_t e = 0;
            while (tmp % f == 0) {
                tmp /= f;
                ++e;
            }
            pf2[cnt2] = f;
            exp2[cnt2] = e;
            ++cnt2;
        }

        /* Generate all divisors of p+1 that are < p. */
        uint64_t divisors[1024];
        size_t div_cnt = 0;
        gen_divisors(pf2, exp2, cnt2, 1, divisors, &div_cnt, p);

        /* Check whether any divisor is a primitive root. */
        bool found = false;
        for (size_t i = 0; i < div_cnt && !found; ++i) {
            uint64_t g = divisors[i];
            if (is_primitive_root(g, p, pf1, cnt1))
                found = true;
        }

        if (found)
            append_candidate(&cache, &cache_len, &cache_cap, (seq_t)p);
    }

    return cache[n - 1];
}
