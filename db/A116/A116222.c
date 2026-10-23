/*
 * A116222  Numbers k such that k+(k+1) is prime and sum of the prime factors
 *          of k and k+1 is also prime.
 * https://oeis.org/A116222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Seq. A116222: k such that k+(k+1) is prime and the sum of the
   prime factors of k and k+1 (with multiplicity) is also prime.
   a(n) is the n‑th such k (1‑based). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/*--- deterministic Miller–Rabin test for 64‑bit integers -----------------*/
static bool is_prime64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t smalls[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(smalls)/sizeof(smalls[0]); ++i) {
        if (n % smalls[i] == 0) return n == smalls[i];
    }

    uint64_t d = n-1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        __uint128_t x = 1, p = a;
        uint64_t e = d;
        while (e) {
            if (e & 1) x = (x * p) % n;
            p = (p * p) % n;
            e >>= 1;
        }
        if (x == 1 || x == n-1) continue;
        bool cont = false;
        for (int r = 1; r < s; ++r) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/*--- simple incremental prime list ---------------------------------------*/
static uint64_t primes_max = 1;            /* highest number tested   */
static size_t primes_cap   = 0;            /* array capacity         */
static size_t primes_len   = 0;            /* number of primes stored*/
static uint32_t *prime_list = NULL;        /* prime table             */

/* ensure prime_list contains all primes up to 'up_to' (inclusive) */
static void ensure_primes(uint64_t up_to)
{
    if (up_to <= primes_max) return;
    if (prime_list == NULL) {
        primes_cap   = 64;
        prime_list   = (uint32_t*)malloc(primes_cap * sizeof(uint32_t));
        if (!prime_list) return;          /* ignore allocation failure */
        prime_list[0] = 2;
        primes_len   = 1;
        primes_max   = 2;
    }
    for (uint64_t cand = primes_max+1; cand <= up_to; ++cand) {
        bool is_p = true;
        double sq = sqrt((double)cand);
        uint64_t limit = (uint64_t)(sq + 0.5);
        for (size_t i = 0; i < primes_len && (uint64_t)prime_list[i] <= limit; ++i) {
            if (cand % prime_list[i] == 0) { is_p = false; break; }
        }
        if (is_p) {
            if (primes_len == primes_cap) {
                primes_cap *= 2;
                uint32_t *tmp = (uint32_t*)realloc(prime_list, primes_cap * sizeof(uint32_t));
                if (!tmp) { prime_list = NULL; primes_len = 0; primes_cap = 0; return; }
                prime_list = tmp;
            }
            prime_list[primes_len++] = (uint32_t)cand;
        }
    }
    primes_max = up_to;
}

/* test primality using the prime list (slow for large numbers) */
static bool is_prime_fast(uint64_t n)
{
    if (n < 2) return false;
    ensure_primes((uint64_t)(sqrt((double)n))+1);
    for (size_t i = 0; i < primes_len; ++i) {
        uint32_t p = prime_list[i];
        if ((uint64_t)p * (uint64_t)p > n) break;
        if (n % p == 0) return n == p;
    }
    /* fallback to Miller–Rabin for remaining large numbers */
    return is_prime64(n);
}

/* sum of prime factors of n (with multiplicity), result in seq_t       */
static seq_t sum_prime_factors(uint64_t n)
{
    seq_t sum = 0;
    if (n <= 1) return sum;
    ensure_primes((uint64_t)(sqrt((double)n))+1);
    uint64_t tmp = n;
    for (size_t i = 0; i < primes_len && (uint64_t)prime_list[i] * (uint64_t)prime_list[i] <= tmp; ++i) {
        uint32_t p = prime_list[i];
        while (tmp % p == 0) {
            sum += (seq_t)p;
            tmp /= p;
        }
    }
    if (tmp > 1) sum += (seq_t)tmp;
    return sum;
}

/*--- sequence function ---------------------------------------------------*/
seq_t a(idx_t n)
{
    /* static cache for already found terms */
    static seq_t *cache   = NULL;
    static idx_t cache_size = 0;
    static idx_t cache_cap  = 0;
    static uint64_t last_k = 1;          /* next candidate to test */

    /* initialise prime list on first call */
    if (prime_list == NULL) ensure_primes(1000);

    /* generate until we have n terms */
    while (cache_size < n) {
        uint64_t k = last_k;
        bool ok = false;
        if (k == 1) {
            /* k == 1 is taken as a term regardless of the sum condition */
            ok = true;
        } else {
            /* 1) k + (k+1) = 2k+1 must be prime */
            uint64_t two_k_plus_one = k + (k + 1);  /* = 2k+1 */
            if (is_prime_fast(two_k_plus_one)) {
                /* 2) sum of prime factors of k and k+1 must be prime */
                seq_t sum_pf = sum_prime_factors(k) + sum_prime_factors(k + 1);
                uint64_t sp_sum = (uint64_t)sum_pf;
                if (is_prime_fast(sp_sum)) ok = true;
            }
        }
        if (ok) {
            /* store into cache */
            if (cache_size >= cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
                seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) { /* allocation failure – give up */ return 0; }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)k;
        }
        ++last_k;
    }
    return cache[n-1];
}
