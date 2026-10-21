/*
 * A231810  Numbers k such that p-1 does not divide k+1 for every prime
 *          divisor p of k.
 * https://oeis.org/A231810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th integer k such that for every prime divisor p
 *            of k, (p-1) does not divide (k+1). Implementation iterates
 *            through integers, testing the condition and caching results for
 *            subsequent calls
 * Range:     u8 n <= 70, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Cached sequence values -------------------------------------------------- */
static seq_t *cache = NULL;       /* dynamically-sized array of a(i) values    */
static size_t cache_len = 0;      /* number of terms computed so far           */
static size_t cache_cap = 0;      /* current allocation capacity of cache      */
static uint64_t next_candidate = 0;   /* last integer examined for property */

/* Prime list for factorisation ------------------------------------------- */
static uint64_t *prime_list = NULL;   /* list of primes discovered so far        */
static size_t prime_list_len = 0;     /* number of primes stored                 */
static size_t prime_list_cap = 0;     /* capacity of prime_list array            */
static uint64_t prime_list_last = 1;  /* largest integer that has been tested    */

/* ------------------------------------------------------------------------ */
/* Simple integer square root (floor) using only integer arithmetic.        */
/* ------------------------------------------------------------------------ */
static uint64_t isqrt_uint64(uint64_t n)
{
    uint64_t low = 0, high = n;
    while (low <= high) {
        uint64_t mid = (low + high) >> 1;
        if (mid > 0 && mid > n / mid) {
            high = mid - 1;
        } else {
            if ((mid + 1) > 0 && (mid + 1) > n / (mid + 1)) {
                return mid;
            }
            low = mid + 1;
        }
    }
    return low;
}

/* ------------------------------------------------------------------------ */
/* Grow a dynamic array of uint64_t primes to at least new_cap elements.   */
/* ------------------------------------------------------------------------ */
static void ensure_primes_capacity(size_t new_cap)
{
    if (prime_list_cap >= new_cap) return;
    size_t cap = prime_list_cap ? prime_list_cap * 2 : 16;
    while (cap < new_cap) cap *= 2;
    uint64_t *tmp = (uint64_t *)realloc(prime_list, cap * sizeof(uint64_t));
    if (!tmp) exit(1);
    prime_list = tmp;
    prime_list_cap = cap;
}

/* ------------------------------------------------------------------------ */
/* Append a prime to the prime list.                                        */
/* ------------------------------------------------------------------------ */
static void add_prime(uint64_t p)
{
    if (prime_list_len == prime_list_cap) {
        size_t new_cap = prime_list_cap ? prime_list_cap * 2 : 16;
        uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
        if (!tmp) exit(1);
        prime_list = tmp;
        prime_list_cap = new_cap;
    }
    prime_list[prime_list_len++] = p;
}

/* ------------------------------------------------------------------------ */
/* Ensure that prime_list contains all primes up to 'limit'.                */
/* ------------------------------------------------------------------------ */
static void ensure_primes_up_to(uint64_t limit)
{
    if (prime_list_last >= limit) return;
    for (uint64_t cand = prime_list_last + 1; cand <= limit; ++cand) {
        if (cand < 2) continue;     /* skip 0,1 */
        bool is_prime = true;
        uint64_t cand_sqrt = isqrt_uint64(cand);
        for (size_t i = 0; i < prime_list_len; ++i) {
            uint64_t p = prime_list[i];
            if (p > cand_sqrt) break;
            if (cand % p == 0) { is_prime = false; break; }
        }
        if (is_prime) add_prime(cand);
    }
    prime_list_last = limit;
}

/* ------------------------------------------------------------------------ */
/* Grow cache array to at least new_cap terms.                              */
/* ------------------------------------------------------------------------ */
static void ensure_cache_capacity(size_t new_cap)
{
    if (cache_cap >= new_cap) return;
    size_t cap = cache_cap ? cache_cap * 2 : 16;
    while (cap < new_cap) cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    if (!tmp) exit(1);
    cache = tmp;
    cache_cap = cap;
}

/* ------------------------------------------------------------------------ */
/* Test whether a given integer k satisfies the sequence property.         */
/* ------------------------------------------------------------------------ */
static bool ok_for_k(uint64_t k)
{
    if (k == 1) return true;  /* 1 is allowed by definition */
    uint64_t orig = k;
    uint64_t temp = k;
    uint64_t lim = isqrt_uint64(k);
    ensure_primes_up_to(lim);

    for (size_t i = 0; i < prime_list_len; ++i) {
        uint64_t p = prime_list[i];
        if (p > lim) break;
        if (temp % p == 0) {
            if ((orig + 1) % (p - 1) == 0) return false;
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1) { /* remaining prime factor */
        if ((orig + 1) % (temp - 1) == 0) return false;
    }
    return true;
}

/* ------------------------------------------------------------------------ */
/* Main function requested by the problem.                                  */
/* ------------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 0 - guard */
    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        ++next_candidate;
        if (ok_for_k(next_candidate)) {
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len++] = (seq_t)next_candidate;
        }
    }
    return cache[n - 1];
}
