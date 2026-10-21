/*
 * A098142  Numbers n such that (p*(q-1)+(p-1)*q)/2 is prime, where p and q
 *          are consecutive odd primes, p<q and p_n is the n-th prime.
 * https://oeis.org/A098142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, mod_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A098142
   a(n) = the n-th index i such that
          (p_i*(p_{i+1}-1) + (p_i-1)*p_{i+1})/2
          is prime, where p_i are the odd primes
          (p_1 = 2, p_2 = 3, …).
   The returned value is the index i (1‑based). */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"
#include "../utils/mod_mul.h"

/* 64‑bit modular multiplication using 128‑bit arithmetic. */

/* 64‑bit modular exponentiation. */
static uint64_t mod_pow(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t res = 1;
    while (exp) {
        if (exp & 1)
            res = mod_mul(res, base, mod);
        base = mod_mul(base, base, mod);
        exp >>= 1;
    }
    return res;
}

/* Deterministic Miller‑Rabin primality test for 64‑bit integers. */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)
        return false;
    static const uint64_t small_primes[] = {
        2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,
        29ULL,31ULL,37ULL,41ULL,43ULL,47ULL
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0)
            return n == p;
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }
    static const uint64_t witnesses[] = { 2ULL, 325ULL, 9375ULL, 28178ULL,
                                          450775ULL, 9780504ULL, 1795265022ULL };
    for (size_t i = 0; i < sizeof(witnesses)/sizeof(witnesses[0]); ++i) {
        uint64_t a = witnesses[i] % n;
        if (a == 0)
            continue;
        uint64_t x = mod_pow(a, d, n);
        if (x == 1 || x == n - 1)
            continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mod_mul(x, x, n);
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite)
            return false;
    }
    return true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Memoisation state */
    static idx_t prime_limit = 2000000;     /* limit for primes (value ≤ limit)   */
    static uint64_t *primes = NULL;         /* array of primes (uint64_t)        */
    static size_t primes_cnt = 0;           /* number of primes generated      */

    static idx_t processed_prime_index = 1; /* index of current p to test (0‑based) */
    static seq_t *vals = NULL;              /* stored indices that satisfy     */
    static size_t vals_cnt = 0;             /* number stored                   */
    static size_t vals_cap = 0;             /* capacity of vals                */

    /* Initialise primes list once */
    if (!primes) {
        build_primes_upto(prime_limit, &primes, &primes_cnt);
    }

    /* Generate at least n values if necessary */
    while (vals_cnt < (size_t)n) {

        /* Ensure we have a pair of consecutive primes */
        while (processed_prime_index + 1 >= primes_cnt) {
            prime_limit *= 2;
            free(primes);
            build_primes_upto(prime_limit, &primes, &primes_cnt);
            if (processed_prime_index >= primes_cnt)
                processed_prime_index = primes_cnt - 1;
        }

        /* Skip the even prime 2 (index 0) – start from index 1 (prime 3) */
        if (processed_prime_index == 0) {
            ++processed_prime_index;
            continue;
        }

        uint64_t p = primes[processed_prime_index];
        uint64_t q = primes[processed_prime_index + 1];

        /* Compute E = p*q - (p+q)/2  (always integer for odd primes) */
        unsigned __int128 prod = (unsigned __int128)p * q;
        uint64_t sum_half = (p + q) / 2;
        uint64_t E = (uint64_t)(prod - sum_half);

        /* Test primality of E */
        if (is_prime_uint64(E)) {
            if (vals_cnt == vals_cap) {
                size_t new_cap = vals_cap == 0 ? 16 : vals_cap * 2;
                seq_t *tmp = (seq_t *)realloc(vals, new_cap * sizeof(seq_t));
                if (!tmp) {
                    /* Allocation failed – abort with zero (unlikely for this
                       problem size). */
                    return 0;
                }
                vals = tmp;
                vals_cap = new_cap;
            }
            /* Store the 1‑based index (primes are 0‑based array) */
            vals[vals_cnt++] = (seq_t)(processed_prime_index + 1);
        }

        ++processed_prime_index;
    }

    return vals[n - 1];
}
