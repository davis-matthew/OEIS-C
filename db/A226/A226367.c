/*
 * A226367  Multiplicative order of the (n+1)-st prime modulo the n-th prime.
 * https://oeis.org/A226367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 6544, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 91 terms through n = 9813 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Multiplicative order of p(n+1) modulo p(n)
   a(n) = smallest positive m with p(n+1)^m ≡ 1 (mod p(n)), n ≥ 1
   where p(k) is the k‑th prime.
   The function uses trial‑division to factor p(n)−1 and reduces the
   order by its prime divisors using fast modular exponentiation.
*/

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

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

static seq_t mod_pow_uint64(seq_t base, uint64_t exp, seq_t mod)
{
    uint64_t b  = (uint64_t)base;
    uint64_t m  = (uint64_t)mod;
    uint64_t result = 1;

    while (exp > 0) {
        if (exp & 1ULL) {
            __uint128_t prod = (__uint128_t)result * b;
            result = (uint64_t)(prod % m);
        }
        exp >>= 1ULL;
        if (exp) {
            __uint128_t prod = (__uint128_t)b * b;
            b = (uint64_t)(prod % m);
        }
    }
    return (seq_t)result;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined for n < 1 */

    /* Retrieve the n‑th and (n+1)‑st primes */
    uint32_t p_n   = get_prime(n);
    uint32_t p_np1 = get_prime(n + 1);

    uint64_t mod  = (uint64_t)p_n;
    uint64_t base = (uint64_t)(p_np1 % mod);

    /* The multiplicative order divides p_n−1 */
    uint64_t order = mod - 1;

    /* Factorize order into its unique prime divisors.
       array size 64 is ample for the range of p_n we need. */
    uint64_t factors[64];
    size_t   fcnt = 0;
    uint64_t temp = order;

    for (uint64_t d = 2; d * d <= temp; ++d) {
        if (temp % d == 0) {
            factors[fcnt++] = d;
            while (temp % d == 0) temp /= d;
        }
    }
    if (temp > 1) factors[fcnt++] = temp;   /* remaining prime factor */

    /* Reduce order by dividing out each prime factor while the condition
       remains true.  This yields the smallest exponent m. */
    for (size_t i = 0; i < fcnt; ++i) {
        uint64_t q = factors[i];
        while (order % q == 0) {
            uint64_t candidate = order / q;
            if (mod_pow_uint64(base, candidate, mod) == 1)
                order = candidate;
            else
                break;
        }
    }

    return (seq_t)order;
}
