/*
 * A099800  Bisection of A002110.
 * https://oeis.org/A099800
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Π_{i=1}^{2n+1} p_i (product of the first 2n+1 primes)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 2, a(1) = 30, a(2) = 2310, a(3) = 510510, …  (OEIS A099800) */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------------- Prime generation ---------------- */
static uint32_t *prime_list   = NULL;   /* array of primes */
static size_t   prime_count   = 0;      /* number of primes stored */
static size_t   prime_cap     = 0;      /* current capacity of prime_list */
static uint32_t next_candidate = 2;     /* next integer to test for primality */

/* Grow the prime array when needed */
static void grow_prime_list(void)
{
    size_t new_cap = prime_cap ? prime_cap * 2 : 64;
    uint32_t *new_arr = (uint32_t *)realloc(prime_list, new_cap * sizeof(uint32_t));
    if (!new_arr) abort();
    prime_list = new_arr;
    prime_cap = new_cap;
}

/* Append a newly found prime to the array */
static void add_prime(uint32_t p)
{
    if (prime_count == prime_cap) grow_prime_list();
    prime_list[prime_count++] = p;
}

/* Ensure that the first 'need' primes are available (prime_count >= need) */
static void ensure_primes_needed(size_t need)
{
    if (prime_count >= need) return;
    /* Trial‑division generation of successive primes */
    for (uint32_t cand = next_candidate; prime_count < need; ++cand)
    {
        bool is_prime = true;
        for (size_t i = 0; i < prime_count && (uint64_t)prime_list[i] * prime_list[i] <= cand; ++i)
        {
            if (cand % prime_list[i] == 0) { is_prime = false; break; }
        }
        if (is_prime) add_prime(cand);
    }
    next_candidate = prime_list[prime_count - 1] + 1;
}

/* ---------------- Cache of sequence values ---------------- */
static seq_t *cache     = NULL;   /* array of computed a(n) values */
static size_t cache_len = 0;      /* number of values currently stored */
static size_t cache_cap = 0;      /* capacity of cache array */

/* Grow the cache array when needed */
static void grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 32;
    seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_arr) abort();
    cache = new_arr;
    cache_cap = new_cap;
}

/* Ensure that a(n) for all 0 <= i <= n are stored in cache */
static void ensure_cache_upto(idx_t n)
{
    if (cache_len > (size_t)n) return;

    /* Number of primes required: 2n + 1 */
    ensure_primes_needed((size_t)(2 * (idx_t)n + 1));

    for (size_t k = cache_len; k <= (size_t)n; ++k)
    {
        seq_t val;
        if (k == 0)
        {   /* first element: just the first prime */
            val = (seq_t)prime_list[0];
        }
        else
        {
            val = cache[k - 1];
            val *= (seq_t)prime_list[2 * k - 1];
            val *= (seq_t)prime_list[2 * k];
        }
        if (cache_len == cache_cap) grow_cache();
        cache[cache_len++] = val;
    }
}

/* ---------------- Public sequence function ---------------- */
seq_t a(idx_t n)
{
    ensure_cache_upto(n);
    return cache[(size_t)n];
}
