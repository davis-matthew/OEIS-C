/*
 * A129998  Let p < q be consecutive primes. Then p is in the sequence if
 *          p+q+p*q is a composite number and if there is a prime number r
 *          such that p+q+r is a divisor of p+q+p*q.
 * https://oeis.org/A129998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   _floor_isqrt_u64, _smallest_prime_factor, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A129998 – primes p such that for consecutive primes p < q,
   S = p + q + p*q is composite, and there is a prime r with
   p + q + r dividing S.  a(1) = 31.  (No simple closed formula.) */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/next_prime.h"          /* uint64_t next_prime(uint64_t) */
#include "../utils/_smallest_prime_factor.h" /* uint64_t _smallest_prime_factor(uint64_t) */
#include "../utils/_floor_isqrt_u64.h"     /* uint64_t _floor_isqrt_u64(uint64_t) */

/* ---------- dynamic storage for found terms ---------- */
static seq_t  *seq_vals      = NULL; /* array of computed terms          */
static size_t seq_count      = 0;    /* number of terms stored           */
static size_t seq_capacity   = 0;    /* allocated size of seq_vals        */

/* ---------- prime table for factorisation ---------- */
static uint64_t *prime_vec  = NULL; /* array of primes up to needed limit */
static size_t   prime_count = 0;    /* number of primes stored           */
static size_t   prime_cap   = 0;    /* allocated capacity for prime_vec  */
static uint64_t last_prime  = 0;    /* largest prime currently in prime_vec */

/* Append a primitive prime to the table */
static void append_prime(uint64_t p)
{
    if (prime_count == prime_cap) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 16;
        uint64_t *tmp = (uint64_t *)realloc(prime_vec, new_cap * sizeof(uint64_t));
        if (!tmp) abort();           /* allocation failure */
        prime_vec = tmp;
        prime_cap = new_cap;
    }
    prime_vec[prime_count++] = p;
    last_prime = p;
}

/* Ensure prime_vec contains all primes up to at least limit */
static void ensure_primes_upto(uint64_t limit)
{
    if (prime_count == 0) {
        append_prime(2);              /* start with the first prime */
    }
    while (last_prime < limit) {
        uint64_t next = next_prime(last_prime);
        append_prime(next);
    }
}

/* Recursive descent to generate all divisors of S from its prime
   factorisation and look for a divisor d > p+q such that
   r = d - (p+q) is prime.  Return true as soon as such a d is found. */
static bool search_divisor(uint64_t current, size_t idx,
                           uint64_t p, uint64_t q,
                           const uint64_t *factors, const uint8_t *exps,
                           size_t fcnt, uint64_t target_sum)
{
    if (idx == fcnt) {
        if (current > target_sum) {
            uint64_t r = current - target_sum;
            if (r > 1 && _smallest_prime_factor(r) == r)
                return true;
        }
        return false;
    }

    uint64_t prime = factors[idx];
    uint8_t  exp   = exps[idx];
    __uint128_t mult = 1;

    for (uint8_t k = 0; k <= exp; ++k) {
        if (search_divisor((uint64_t)(current * mult), idx + 1,
                           p, q, factors, exps, fcnt, target_sum))
            return true;
        mult *= prime;
    }
    return false;
}

/* Check whether S has a divisor satisfying the required condition. */
static bool has_valid_r(uint64_t S, uint64_t p, uint64_t q,
                        const uint64_t *factors, const uint8_t *exps,
                        size_t fcnt)
{
    return search_divisor(1, 0, p, q, factors, exps, fcnt, p + q);
}

/* Main sequence function --------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;              /* undefined for n < 1 */

    /* Initialise prime table if empty */
    if (prime_count == 0)
        ensure_primes_upto(2);

    /* Current p of the consecutive pair p < q */
    static uint64_t current_p = 2;

    while (seq_count < (size_t)n) {
        uint64_t p = current_p;
        uint64_t q = next_prime(p);

        /* Ensure prime table contains q for future factorisations */
        if (q > last_prime)
            ensure_primes_upto(q);

        /* Compute S = p + q + p*q  (use 64‑bit arithmetic) */
        uint64_t S = p + q + p * q;

        /* If S is composite, search for a suitable divisor */
        if (_smallest_prime_factor(S) != S) {                 /* composite */
            /* Factor S */
            ensure_primes_upto((uint64_t)_floor_isqrt_u64(S));

            uint64_t temp = S;
            uint64_t factors[64];
            uint8_t   exps[64];
            size_t    fcnt = 0;

            for (size_t i = 0; i < prime_count && prime_vec[i] * prime_vec[i] <= temp; ++i) {
                uint64_t pr = prime_vec[i];
                if (temp % pr == 0) {
                    factors[fcnt] = pr;
                    uint8_t e = 0;
                    while (temp % pr == 0) {
                        temp /= pr;
                        ++e;
                    }
                    exps[fcnt] = e;
                    ++fcnt;
                }
            }
            if (temp > 1) {                          /* remaining prime factor */
                factors[fcnt] = temp;
                exps[fcnt]    = 1;
                ++fcnt;
            }

            if (has_valid_r(S, p, q, factors, exps, fcnt)) {
                /* Store p in the sequence */
                if (seq_count == seq_capacity) {
                    size_t new_cap = seq_capacity ? seq_capacity * 2 : 16;
                    seq_t *new_arr = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
                    if (!new_arr) abort();
                    seq_vals = new_arr;
                    seq_capacity = new_cap;
                }
                seq_vals[seq_count++] = (seq_t)p;
            }
        }

        /* Advance to next consecutive pair */
        current_p = q;
    }

    return seq_vals[n - 1];
}
