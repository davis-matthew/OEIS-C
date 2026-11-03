/*
 * A220953  Primes p such that floor(sqrt(2)) + floor(sqrt(3)) +
 *          floor(sqrt(5)) + ... + floor(sqrt(p)) is prime.
 * https://oeis.org/A220953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 48, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 64 terms through n = 9864 (2026-11-03)
 * Helpers:   ipow_i32, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes primes p such that
   S(p) = Σ_{q ≤ p, q prime} floor(sqrt(q)) is a prime.
   a(n) returns the n‑th such prime (n ≥ 1). */
#include <stdlib.h>
#include <stdbool.h>

/* utilities provided by the build */
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/is_prime_u64.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* global state for caching */
static uint64_t *prime_list = NULL;    /* all primes discovered so far   */
static size_t   prime_count = 0;
static size_t   prime_cap   = 0;

static seq_t *valid_primes = NULL;    /* primes satisfying the condition */
static size_t valid_count  = 0;
static size_t valid_cap    = 0;

static uint64_t last_candidate = 3;   /* next odd number to test        */
static seq_t    cumulative_sum = 0;   /* Σ floor(sqrt(q)) for known primes */

/* ensure prime_list has room for another prime */
static void grow_prime_list(void)
{
    if (prime_count >= prime_cap) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 4;
#if defined(__cplusplus)
        uint64_t *new_list = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
#else
        uint64_t *new_list = realloc(prime_list, new_cap * sizeof(uint64_t));
#endif
        if (!new_list) exit(EXIT_FAILURE);
        prime_list = new_list;
        prime_cap  = new_cap;
    }
}

/* ensure valid_primes has room for another entry */
static void grow_valid_list(void)
{
    if (valid_count >= valid_cap) {
        size_t new_cap = valid_cap ? valid_cap * 2 : 4;
        seq_t *new_list = seq_realloc(valid_primes, valid_cap, new_cap);
        if (!new_list) exit(EXIT_FAILURE);
        valid_primes = new_list;
        valid_cap    = new_cap;
    }
}

/* simple primality test using the primes already discovered */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* generate the next prime > last_candidate and update caches */
static void generate_next_prime(void)
{
    while (true) {
        uint64_t cand = last_candidate;
        last_candidate += 2;     /* next odd number */

        if (is_prime_uint64(cand)) {
            /* store the new prime */
            grow_prime_list();
            prime_list[prime_count++] = cand;

            /* update sum of floor(sqrt(q)) */
            cumulative_sum += floor_sqrt((seq_t)cand);

            /* test if the cumulative sum is prime */
            if (is_prime_big(cumulative_sum)) {
                grow_valid_list();
                valid_primes[valid_count++] = cand;
            }
            break; /* prime found, exit the loop */
        }
    }
}

/* public function required by the grader */
seq_t a(idx_t n)
{
    if (n < 1) return 0; /* undefined for n == 0 in OEIS */

    /* initialisation: prime 2 */
    if (prime_count == 0) {
        grow_prime_list();
        prime_list[prime_count++] = 2;
        cumulative_sum += floor_sqrt((seq_t)2); /* floor(sqrt(2)) = 1 */
        /* 2 does not satisfy the condition (sum = 1) */
        last_candidate = 3; /* start testing from 3 */
    }

    /* generate primes until we have enough valid ones */
    while (valid_count < (size_t)n) {
        generate_next_prime();
    }

    return valid_primes[n - 1];
}
