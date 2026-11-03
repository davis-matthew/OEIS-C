/*
 * A059955  a(n) = floor( prime(n)!/lcm(1..prime(n)) ) modulo prime(n).
 * https://oeis.org/A059955
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 64 terms through n = 65 (2026-11-01)
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A059955: a(n) = floor(prime(n)! / lcm(1..prime(n))) mod prime(n) */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

static uint64_t *prime_cache = NULL;
static size_t prime_cache_cnt = 0;
static idx_t prime_cache_limit = 0;

/* modular exponentiation using 64‑bit arithmetic to avoid overflow */
static seq_t modpow_uint64(seq_t base, idx_t exp, seq_t mod)
{
    uint64_t result = 1ULL;
    uint64_t b      = (uint64_t)base % (uint64_t)mod;
    uint64_t m      = (uint64_t)mod;
    while (exp > 0) {
        if (exp & 1U)
            result = (result * b) % m;
        b = (b * b) % m;
        exp >>= 1;
    }
    return (seq_t)result;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;           /* offset starts at 2 */

    /* obtain the nth prime (1‑based indexing) */
    seq_t p_seq = (seq_t)get_prime(n);
    uint64_t p  = (uint64_t)p_seq;

    /* ensure we have all primes < p in the cache */
    if (p - 1 > prime_cache_limit) {
        uint64_t *tmp;
        size_t   tmp_cnt;
        build_primes_upto(p - 1, &tmp, &tmp_cnt);
        if (prime_cache)
            free(prime_cache);
        prime_cache       = tmp;
        prime_cache_cnt   = tmp_cnt;
        prime_cache_limit = p - 1;
    }

    /* compute denom = product_{q<p} q^{floor(log_q p)} (mod p) */
    seq_t denom_mod = 1;
    for (size_t i = 0; i < prime_cache_cnt && prime_cache[i] < p; ++i) {
        uint64_t q = prime_cache[i];
        /* find exponent e: largest e with q^e <= p */
        size_t e = 0;
        uint64_t q_pow = q;
        while (q_pow <= p) {
            ++e;
            if (q_pow > p / q)
                break;
            q_pow *= q;
        }
        seq_t pow_mod = modpow_uint64((seq_t)q, e, p_seq);
        denom_mod = (seq_t)(((uint64_t)denom_mod * (uint64_t)pow_mod) % (uint64_t)p);
    }

    /* numerator is (p-1)! mod p, which by Wilson is p-1 */
    seq_t numerator_mod = (seq_t)(p - 1);

    /* inverse of denom_mod modulo p */
    seq_t inv_denom = modpow_uint64(denom_mod, (idx_t)(p - 2), p_seq);

    /* final answer = numerator_mod * inv_denom mod p */
    uint64_t mult = ((uint64_t)numerator_mod * (uint64_t)inv_denom) % (uint64_t)p;
    return (seq_t)mult;
}
