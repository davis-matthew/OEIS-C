/*
 * A105894  Primes for which -22 is a primitive root.
 * https://oeis.org/A105894
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Search: primes p (>2) such that -22 is a primitive root modulo p.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

/* modular exponentiation using 128‑bit arithmetic */
static uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp) {
        if (exp & 1U) result = (uint64_t)((__int128)result * b % mod);
        b = (uint64_t)((__int128)b * b % mod);
        exp >>= 1U;
    }
    return result;
}

/* test whether -22 is a primitive root modulo prime p */
static bool is_primitive_root(uint64_t p, uint32_t *prime_list, size_t prime_cnt)
{
    if (p <= 2) return false;
    uint64_t a = (p - 22 % p) % p;
    if (a == 0) return false;          /* p divides 22 */

    uint64_t m = p - 1;
    uint64_t tmp = m;
    uint32_t *factors = NULL;
    size_t fcnt = 0, fcap = 0;

    /* factor m */
    for (size_t i = 0; i < prime_cnt && (uint64_t)prime_list[i] * prime_list[i] <= tmp; ++i) {
        uint32_t q = prime_list[i];
        if (tmp % q == 0) {
            if (fcnt == fcap) { fcap = fcap ? fcap * 2 : 4;
                                factors = (uint32_t *)realloc(factors, fcap * sizeof(uint32_t)); }
            factors[fcnt++] = q;
            while (tmp % q == 0) tmp /= q;
        }
    }
    if (tmp > 1) {
        if (fcnt == fcap) { fcap = fcap ? fcap * 2 : 4;
                            factors = (uint32_t *)realloc(factors, fcap * sizeof(uint32_t)); }
        factors[fcnt++] = (uint32_t)tmp;
    }

    /* check order */
    bool ok = true;
    for (size_t i = 0; i < fcnt && ok; ++i) {
        uint64_t pow = (p - 1) / (uint64_t)factors[i];
        if (mod_pow_uint64(a, pow, p) == 1) ok = false;
    }

    free(factors);
    return ok;
}

/* prime generator and sequence memoisation */
seq_t a(idx_t n)
{
    static uint32_t *prime_list = NULL;          /* list of all primes found so far */
    static size_t prime_cnt = 0, prime_cap = 0;
    static uint64_t last_checked = 1;            /* last value examined for primality */

    static uint32_t *good = NULL;                /* list of primes for which -22 is a primitive root */
    static size_t good_cnt = 0, good_cap = 0;

    /* initialise prime list with 2 */
    if (prime_cnt == 0) {
        prime_cap = 16;
        prime_list = (uint32_t *)malloc(prime_cap * sizeof(uint32_t));
        prime_list[0] = 2;
        prime_cnt = 1;
    }

    /* compute more good primes until we have at least n of them */
    while (good_cnt < (size_t)n) {
        uint64_t p;

        /* find next prime */
        while (true) {
            ++last_checked;
            if (last_checked <= 2) { p = 2; break; }
            if ((last_checked & 1U) == 0) continue;   /* skip even numbers */

            bool prime = true;
            uint64_t limit = (uint64_t)sqrt((double)last_checked);
            for (size_t i = 0; i < prime_cnt && prime_list[i] <= limit; ++i) {
                if (last_checked % prime_list[i] == 0) { prime = false; break; }
            }
            if (!prime) continue;

            /* add to prime list */
            if (prime_cnt == prime_cap) { prime_cap *= 2;
                                          prime_list = (uint32_t *)realloc(prime_list, prime_cap * sizeof(uint32_t)); }
            prime_list[prime_cnt++] = (uint32_t)last_checked;
            p = last_checked;
            break;
        }

        /* skip small primes or those that divide 22 */
        if (p <= 2 || p == 11) continue;

        /* test primitive root condition */
        if (is_primitive_root(p, prime_list, prime_cnt)) {
            if (good_cnt == good_cap) { good_cap = good_cap ? good_cap * 2 : 8;
                                         good = (uint32_t *)realloc(good, good_cap * sizeof(uint32_t)); }
            good[good_cnt++] = (uint32_t)p;
        }
    }

    return (seq_t)good[n - 1];
}
