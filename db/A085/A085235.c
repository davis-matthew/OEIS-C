/*
 * A085235  (Greatest power of greatest prime factor of n) > square root(n).
 * https://oeis.org/A085235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th positive integer m (>1) such that the greatest
 *            power of its greatest prime factor exceeds sqrt(m)
 * Range:     u8 n <= 13, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* static storage for the sequence cache */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static size_t cache_len  = 0;

/* static storage for the list of primes */
static uint64_t *primes   = NULL;
static size_t   prime_cap = 0;
static size_t   prime_cnt = 0;
static uint64_t sieve_limit = 0;            /* highest number that has been
                                               prime‑sieved so far */

/* ---------- prime utilities ------------------------------------------ */
static void add_prime(uint64_t p)
{
    if (prime_cnt == prime_cap) {
        size_t new_cap = prime_cap ? prime_cap << 1 : 8;
        uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
        if (!tmp) abort();
        primes = tmp;
        prime_cap = new_cap;
    }
    primes[prime_cnt++] = p;
}

/* Ensure that all primes up to 'limit' have been generated.            */
static void ensure_primes_upto(uint64_t limit)
{
    if (sieve_limit >= limit) return;

    if (prime_cnt == 0) {
        add_prime(2);
        sieve_limit = 2;
    }

    for (uint64_t cand = sieve_limit + 1; cand <= limit; ++cand) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = primes[i];
            if (p > cand / p) break;  /* p*p > cand */
            if (cand % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) add_prime(cand);
    }
    sieve_limit = limit;
}

/* ---------- cache utilities ------------------------------------------ */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 32;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* ---------- test if m satisfies the condition ------------------------ */
static bool good_candidate(uint64_t m)
{
    uint64_t tmp = m;
    uint64_t largest_p = 0;
    unsigned largest_exp = 0;

    /* factorisation using the prime list */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if (p > tmp / p) break;           /* p*p > tmp */
        if (tmp % p == 0) {
            unsigned cnt = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++cnt;
            }
            if (p > largest_p) {
                largest_p = p;
                largest_exp = cnt;
            }
        }
    }

    /* remaining prime factor */
    if (tmp > 1) {
        if (tmp > largest_p) {
            largest_p = tmp;
            largest_exp = 1;
        }
    }

    /* compute largest_p ^ largest_exp */
    seq_t power = 1;
    for (unsigned i = 0; i < largest_exp; ++i)
        power *= (seq_t)largest_p;

    /* check if power^2 > m (equivalent to power > sqrt(m)) */
    seq_t sq = power * power;
    return sq > (seq_t)m;
}

/* ---------- main sequence function ---------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* safe guard, offset is 1 */

    if ((size_t)n <= cache_len)
        return cache[n-1];

    /* initialise prime list if needed */
    if (prime_cnt == 0)
        ensure_primes_upto(2);

    static idx_t max_tested = 1;    /* highest integer already examined */

    while (cache_len < (size_t)n) {
        ++max_tested;
        ensure_primes_upto((uint64_t)max_tested);

        if (good_candidate((uint64_t)max_tested)) {
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len++] = (seq_t)max_tested;
        }
    }
    return cache[n-1];
}
