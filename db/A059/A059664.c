/*
 * A059664  Primes p such that x^18 = 2 has no solution mod p, but x^6 = 2 has
 *          a solution mod p.
 * https://oeis.org/A059664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 111, u32 n <= 340, u64 n <= 340, u128 n <= 340,
 *            big any n
 * Helpers:   grow_uint64_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A059664 : primes p such that x^18 ≡ 2 (mod p) has no solution
   while x^6 ≡ 2 (mod p) has a solution.
   The algorithm tests primes congruent to 1 (mod 18), finds the
   multiplicative order of 2 modulo each prime, and accepts those for
   which the order divides (p−1)/6 but does not divide (p−1)/18.
   All operations are performed in 64‑bit arithmetic; the result is
   cast to seq_t. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/grow_uint64_array.h"

/* 64‑bit modular multiplication (uses compiler extension __int128) */
static inline uint64_t mod_mul64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((unsigned __int128)a * b % mod);
}

/* 64‑bit modular exponentiation by squaring */
static uint64_t mod_pow64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    while (exp) {
        if (exp & 1) result = mod_mul64(result, base, mod);
        base = mod_mul64(base, base, mod);
        exp >>= 1;
    }
    return result;
}

/* Compute the multiplicative order of 2 modulo prime p.
   The list of known primes (up to sqrt(p)) is supplied in the array 'primes'. */
static uint64_t order_of_two(uint64_t p, uint64_t *primes, size_t primes_cnt)
{
    uint64_t t = p - 1;
    uint64_t ord = t;
    /* factor t using known primes */
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t q = primes[i];
        if (q > t / q) break;       /* q*q > t */
        if (t % q == 0) {
            /* remove all occurrences of q from t */
            while (t % q == 0) t /= q;
            /* reduce ord by q while it remains valid */
            while (ord % q == 0) {
                if (mod_pow64(2, ord / q, p) != 1) break;
                ord /= q;
            }
        }
    }
    if (t > 1) {
        /* remaining factor t is prime */
        uint64_t q = t;
        while (ord % q == 0) {
            if (mod_pow64(2, ord / q, p) != 1) break;
            ord /= q;
        }
    }
    return ord;
}

/* Main sequence function required by the build environment */
seq_t a(idx_t n)
{
    /* static data */
    static bool initialized = false;
    static uint64_t *primes = NULL;
    static size_t primes_cnt = 0, primes_cap = 0;
    static uint64_t *seq = NULL;
    static size_t seq_cnt = 0, seq_cap = 0;

    if (!initialized) {
        /* initialise empty arrays */
        primes_cnt = 0; primes_cap = 0; primes = NULL;
        seq_cnt = 0; seq_cap = 0; seq = NULL;
        /* seed primes with the first two primes */
        grow_uint64_array(&primes, &primes_cnt, &primes_cap);
        primes[primes_cnt++] = 2;
        grow_uint64_array(&primes, &primes_cnt, &primes_cap);
        primes[primes_cnt++] = 3;
        initialized = true;
    }

    /* generate sequence values until we have at least n */
    uint64_t cand = (seq_cnt == 0) ? 5 : seq[seq_cnt - 1] + 2; /* start after last chosen prime, odd numbers only */
    while (seq_cnt < (size_t)n) {
        /* primality test of cand */
        bool is_prime = true;
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint64_t q = primes[i];
            if (q > cand / q) break; /* q*q > cand */
            if (cand % q == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            /* add cand to prime list */
            grow_uint64_array(&primes, &primes_cnt, &primes_cap);
            primes[primes_cnt++] = cand;

            /* apply sequence filter */
            if (cand > 3 && ((cand - 1) % 18) == 0) {
                uint64_t m = (cand - 1) / 6;      /* (p-1)/6  */
                uint64_t ord = order_of_two(cand, primes, primes_cnt);
                if ((m % ord) == 0 && ((m / 3) % ord) != 0) {
                    /* prime satisfies the condition */
                    grow_uint64_array(&seq, &seq_cnt, &seq_cap);
                    seq[seq_cnt++] = cand;
                }
            }
        }
        cand += 2; /* skip even numbers */
    }

    return (seq_t)seq[n - 1];
}
