/*
 * A036467  a(n) + a(n-1) = n-th prime.
 * https://oeis.org/A036467
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1; for n ≥ 1: a(n) = p_n – a(n‑1),
   where p_n is the n‑th prime (1‑based). */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/* ---------- Prime cache ---------- */
static uint32_t *prime_cache = NULL;   /* prime_cache[0] = 2, prime_cache[1] = 3, … */
static size_t   prime_cap   = 0;
static size_t   prime_count = 0;

/* Ensure that prime_cache holds at least k primes (1‑based). */
static void ensure_prime_k(size_t k)
{
    if (prime_count >= k)
        return;

    if (prime_cap == 0) {
        prime_cap   = 16;
        prime_cache = (uint32_t *)malloc(prime_cap * sizeof(uint32_t));
    }

    uint32_t candidate = prime_count ? prime_cache[prime_count - 1] + 1 : 2;
    if (candidate <= 2)
        candidate = 2;
    else if ((candidate & 1u) == 0)
        ++candidate;

    for (;;) {
        if (prime_count >= k)
            break;

        bool is_prime = true;
        for (size_t i = 0; i < prime_count; ++i) {
            uint32_t p = prime_cache[i];
            if ((uint64_t)p * p > candidate)
                break;
            if (candidate % p == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            if (prime_count == prime_cap) {
                prime_cap *= 2;
                prime_cache = (uint32_t *)realloc(prime_cache,
                                                  prime_cap * sizeof(uint32_t));
            }
            prime_cache[prime_count++] = candidate;
        }

        candidate += 2;  /* check next odd number */
    }
}

/* ---------- a(n) cache ---------- */
static seq_t *a_cache = NULL; /* a_cache[i] = a(i) */
static size_t a_cap   = 0;
static size_t a_size  = 0;    /* number of entries currently stored */

/* Ensure a_cache contains values up to index n inclusive */
static void ensure_a_up_to(idx_t n)
{
    if (a_size == 0) {              /* initialise with a(0)=1 */
        a_cap   = 4;
        a_cache = (seq_t *)malloc(a_cap * sizeof(seq_t));
        a_cache[0] = (seq_t)1;
        a_size   = 1;
    }

    if (n + 1 <= a_size)
        return;

    size_t need = (size_t)n + 1;
    if (a_cap < need) {
        a_cap = need * 2;
        a_cache = (seq_t *)realloc(a_cache, a_cap * sizeof(seq_t));
    }

    for (size_t i = a_size; i <= (size_t)n; ++i) {
        size_t prime_index = i;          /* 1‑based prime index */
        ensure_prime_k(prime_index);
        seq_t prime = (seq_t)prime_cache[prime_index - 1];
        seq_t prev  = a_cache[i - 1];
        a_cache[i] = prime - prev;
    }

    a_size = need;
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;              /* a(0) = 1 */
    ensure_a_up_to(n);
    return a_cache[(size_t)n];
}
