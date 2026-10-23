/*
 * A139316  An integer k, k>=2, is in the sequence if A001222(k) (the sum of
 *          the exponents in the prime factorization of k) divides A008472(k)
 *          (the sum of the distinct primes dividing k).
 * https://oeis.org/A139316
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 137, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _smallest_prime_factor, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements A139316.
   An integer k ≥ 2 is in the sequence if Ω(k) (A001222) divides
   the sum of its distinct prime divisors, rad(k) (A008472).
   The function returns the n‑th term (n ≥ 1), with offset 1.  */

/* ---------- dependencies ---------- */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* small prime factor routine (utility available in the build) */
#include "../utils/_smallest_prime_factor.h"

/* re‑allocation utility */
#include "../utils/seq_realloc.h"

/* ---------- static memoisation state ---------- */
static seq_t *cache      = NULL;   /* array of sequence terms            */
static size_t cache_len  = 0;      /* how many terms are stored          */
static size_t cache_cap  = 0;      /* current capacity of @cache         */
static uint64_t max_checked = 1;   /* largest integer that has been examined */

/* ---------- helper: grow the cache array ---------- */
static void
ensure_cache_capacity(size_t new_cap)
{
    if (new_cap <= cache_cap)
        return;

    seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
    if (!tmp)
        abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* ---------- test whether a given integer k satisfies the condition ---------- */
static bool
is_in_sequence(uint64_t k)
{
    seq_t sumExp   = 0;   /* Ω(k), the sum of exponents */
    seq_t sumPrime = 0;   /* sum of distinct primes    */

    while (k > 1) {
        uint64_t p = _smallest_prime_factor(k);  /* smallest prime factor of k */
        uint64_t e = 0;
        while (k % p == 0) {
            k /= p;
            e++;
        }
        sumExp   += e;
        sumPrime += (seq_t)p;
    }
    /* k≥2 ensures sumExp>0, but guard against the unlikely case of 0 */
    return (sumExp > (seq_t)0) && (sumPrime % sumExp == (seq_t)0);
}

/* ---------- grow the cache until at least n terms are stored ---------- */
static void
ensure_cache_size(idx_t n)
{
    while (cache_len < (size_t)n) {
        ++max_checked;                     /* next integer to test      */
        if (is_in_sequence(max_checked)) {
            if (cache_len == cache_cap)
                ensure_cache_capacity(cache_cap ? cache_cap * 2 : 4);
            cache[cache_len++] = (seq_t)max_checked;
        }
    }
}

/* ---------- public sequence function ---------- */
seq_t
a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;        /* undefined for n<1 */
    ensure_cache_size(n);
    return cache[n - 1];
}
