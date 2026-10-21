/*
 * A230357  Numbers n such that digit sum of n equals digit sum of sopf(n)
 *          (sum of the distinct prime factors of n).
 * https://oeis.org/A230357
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A230357: a(n) is the n‑th positive integer such that
 * digit_sum(n) = digit_sum(sopf(n)) where sopf(n) is the sum of the
 * distinct prime factors of n.  Composite numbers only are kept, as
 * prime numbers trivially satisfy the equality but are excluded from
 * the sequence.  This implementation searches the integers in order,
 * stores found terms in a static cache, and grows the cache lazily. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* ----- digit sum on an unsigned 64‑bit integer ----- */
static inline seq_t digit_sum_uint64(uint64_t v)
{
    uint16_t sum = 0;
    while (v) {
        sum += (uint16_t)(v % 10);
        v /= 10;
    }
    return (seq_t)sum;
}

/* ----- integer square root (floor) on uint64_t ----- */
static inline uint64_t isqrt_uint64(uint64_t n)
{
    if (n < 2) return n;
    uint64_t x = n, y = (x + 1) >> 1;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

/* ----- prime storage and generation ----- */
static uint64_t *primes      = NULL;      /* ascending list of primes   */
static size_t   primes_cnt   = 0;         /* number of primes stored   */
static uint64_t primes_limit = 1;        /* highest integer already checked */

/* Extend the prime list to cover all primes up to at least 'limit' */
static void ensure_primes_up_to(uint64_t limit)
{
    if (limit <= primes_limit) return;
    for (uint64_t x = primes_limit + 1; x <= limit; ++x) {
        bool is_prime = true;
        for (size_t i = 0; i < primes_cnt && primes[i] <= x / primes[i]; ++i) {
            if (x % primes[i] == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            uint64_t *tmp = (uint64_t *)realloc(primes,
                                                (primes_cnt + 1) * sizeof(uint64_t));
            if (!tmp) abort();                /* unlikely in tests      */
            primes = tmp;
            primes[primes_cnt++] = x;
        }
    }
    primes_limit = limit;
}

/* ----- primality test using the prime table ----- */
static inline bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    uint64_t lim = isqrt_uint64(n);
    ensure_primes_up_to(lim);
    for (size_t i = 0; i < primes_cnt && primes[i] <= lim; ++i) {
        if (n % primes[i] == 0) return false;
    }
    return true;
}

/* ----- sum of distinct prime factors (sopf) ----- */
static uint64_t sopf(uint64_t n)
{
    if (n < 2) return 0;
    uint64_t temp = n;
    uint64_t sum  = 0;
    uint64_t lim   = isqrt_uint64(n);
    ensure_primes_up_to(lim);
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (p > temp / p) break;
        if (temp % p == 0) {
            sum += p;
            do { temp /= p; } while (temp % p == 0);
        }
    }
    if (temp > 1) sum += temp;          /* remaining prime factor > sqrt(n) */
    return sum;
}

/* ----- cache storage for found terms ----- */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;          /* number of terms stored     */
static size_t cache_cap  = 0;          /* capacity of cache array    */
static uint64_t current  = 0;          /* last integer examined      */

/* Grow the cache array to at least 'needed' elements */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? 2 * cache_cap : 8;
    if (new_cap < needed) new_cap = needed;
    seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_arr) abort();             /* unlikely in tests            */
    cache = new_arr;
    cache_cap = new_cap;
}

/* Ensure that the cache contains at least 'count' terms  */
static void ensure_n_terms(idx_t count)
{
    while (cache_len < (size_t)count) {
        ++current;                                /* candidate integer      */
        if (is_prime_uint64(current))              /* skip primes            */
            continue;
        seq_t ds1 = digit_sum_uint64(current);
        uint64_t s = sopf(current);
        seq_t ds2 = digit_sum_uint64(s);
        if (ds1 == ds2) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)current;   /* store composite term   */
        }
    }
}

/* ----- public function required by the unit tests ----- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1; undefined for n==0   */
    ensure_n_terms(n);
    return cache[n - 1];
}
