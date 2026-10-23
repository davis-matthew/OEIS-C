/*
 * A160690  a(1)=1. a(n) = the smallest integer > a(n-1) such that d(a(n)) =
 *          d(a(n) - a(n-1)), where d(m) = the number of divisors of m.
 * https://oeis.org/A160690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS sequence A160690:
   a(1)=1; for n>1, a(n) is the smallest integer > a(n‑1) such that
   d(a(n)) = d(a(n) – a(n‑1)), where d(k) is the number of divisors of k. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- divisor‑count helpers ---------- */
static uint64_t *prime_list   = NULL;   /* stored primes */
static size_t   prime_count  = 0;
static size_t   prime_cap    = 0;

/* append the next prime to prime_list */
static void grow_primes(void)
{
    uint64_t cand = (prime_count == 0 ? 2 : prime_list[prime_count - 1] + 1);
    for (;; ++cand) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_count && prime_list[i] * prime_list[i] <= cand; ++i) {
            if (cand % prime_list[i] == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (prime_count == prime_cap) {
                size_t new_cap = prime_cap ? prime_cap * 2 : 16;
                uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
                if (!tmp) abort();                /* out of memory */
                prime_list = tmp;
                prime_cap  = new_cap;
            }
            prime_list[prime_count++] = cand;
            break;
        }
    }
}

/* ensure primes up to limit exist in the list */
static void ensure_primes_upto(uint64_t limit)
{
    while (prime_count == 0 || prime_list[prime_count - 1] <= limit)
        grow_primes();
}

/* divisor count of a 64‑bit integer */
static uint64_t divisor_count_u64(uint64_t n)
{
    if (n == 0) return 0;
    uint64_t sqrt_limit = 0;
    while ((sqrt_limit + 1) * (sqrt_limit + 1) <= n)
        ++sqrt_limit;
    ensure_primes_upto(sqrt_limit);

    uint64_t res = 1;
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > n) break;
        if (n % p == 0) {
            uint64_t exp = 0;
            while (n % p == 0) {
                n /= p;
                ++exp;
            }
            res *= (exp + 1);
        }
    }
    if (n > 1) res *= 2;   /* remaining prime factor */
    return res;
}

/* ---------- sequence cache ---------- */
static seq_t *seq_cache      = NULL;
static size_t seq_cache_size = 0;
static size_t seq_cache_cap  = 0;

static void grow_seq_cache(void)
{
    if (seq_cache_size >= seq_cache_cap) {
        size_t new_cap = seq_cache_cap ? seq_cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                /* out of memory */
        seq_cache = tmp;
        seq_cache_cap = new_cap;
    }
}

/* ---------- public function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* undefined, guard */

    /* initialise cache with a(1)=1 once */
    if (seq_cache_size == 0) {
        grow_seq_cache();
        seq_cache[0] = (seq_t)1;
        seq_cache_size = 1;   /* capacity already 16 after grow_seq_cache() */
    }

    /* return if already computed */
    if ((idx_t)seq_cache_size >= n) return seq_cache[n - 1];

    /* compute missing terms up to n */
    while ((idx_t)seq_cache_size < n) {
        seq_t prev = seq_cache[seq_cache_size - 1];
        seq_t cand = prev + 1;
        while (1) {
            seq_t diff = cand - prev;
            uint64_t d_cand = divisor_count_u64((uint64_t)cand);
            uint64_t d_diff = divisor_count_u64((uint64_t)diff);
            if (d_cand == d_diff) break;
            ++cand;
        }
        grow_seq_cache();
        seq_cache[seq_cache_size] = cand;
        ++seq_cache_size;
    }
    return seq_cache[n - 1];
}
