/*
 * A046301  Product of 3 successive primes.
 * https://oeis.org/A046301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = p_n * p_{n+1} * p_{n+2} where p_k is the k‑th prime (p₁ =
 *            2)
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 256, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Prime storage */
static uint64_t *prime_cache = NULL;
static size_t prime_cnt = 0;
static size_t prime_cap = 0;

/* Ensure prime_cache has at least 'need' slots. */
static void ensure_cap(size_t need)
{
    if (need <= prime_cap) return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    uint64_t *tmp = (uint64_t *)realloc(prime_cache, new_cap * sizeof(uint64_t));
    if (!tmp) exit(1);  /* allocation failure */
    prime_cache = tmp;
    prime_cap   = new_cap;
}

/* Test primality of candidate using primes already stored. */
static bool is_prime_uint64(uint64_t candidate)
{
    if (candidate < 2) return false;
    if (candidate % 2 == 0) return candidate == 2;
    for (size_t i = 0; i < prime_cnt; ++i)
    {
        uint64_t p = prime_cache[i];
        /* stop when p*p > candidate; use division to avoid overflow */
        if (candidate / p < p) break;
        if (candidate % p == 0) return false;
    }
    return true;
}

/* Generate primes until we have at least `need` primes in prime_cache. */
static void ensure_primes_upto(size_t need)
{
    if (prime_cnt >= need) return;
    ensure_cap(need);

    uint64_t cand = prime_cnt ? prime_cache[prime_cnt-1] + 1 : 2;
    while (prime_cnt < need)
    {
        if (cand > 2 && (cand & 1) == 0)
        {
            ++cand;
            continue;
        }
        if (is_prime_uint64(cand))
        {
            prime_cache[prime_cnt++] = cand;
        }
        ++cand;
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined for n < 1 */

    /* We need primes p_n, p_{n+1}, p_{n+2} → n+2 primes total. */
    size_t need = (size_t)(n + 2);
    ensure_primes_upto(need);

    seq_t p1 = (seq_t)prime_cache[n-1];
    seq_t p2 = (seq_t)prime_cache[n];
    seq_t p3 = (seq_t)prime_cache[n+1];
    return p1 * p2 * p3;
}
