/*
 * A072225  Numbers k such that prime(k) + prime(k+1) + prime(k+2) is prime.
 * https://oeis.org/A072225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 1466, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: collect k where sum of the k‑th, (k+1)‑th and (k+2)‑th primes is prime. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime.h"

static idx_t   last_k_checked = 1;   /* next k to evaluate (1‑based) */
static seq_t *checked_cache   = NULL;/* array of k that satisfy the condition */
static size_t cache_cap     = 0;
static size_t cache_size    = 0;

/* Ensure checked_cache has at least 'need' terms. */
static void ensure_terms(idx_t need)
{
    while (cache_size < need)
    {
        idx_t k = last_k_checked;

        uint32_t p1 = get_prime(k);
        uint32_t p2 = get_prime(k + 1);
        uint32_t p3 = get_prime(k + 2);

        seq_t sum = (seq_t)p1 + p2 + p3;

        if (is_prime((idx_t)sum))
        {
            if (cache_cap == 0)
            {
                cache_cap = 256;
                checked_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
            }
            else if (cache_size == cache_cap)
            {
                cache_cap <<= 1;
                checked_cache = (seq_t *)realloc(checked_cache, cache_cap * sizeof(seq_t));
            }
            checked_cache[cache_size++] = (seq_t)k;
        }
        ++last_k_checked;
    }
}

/* External routine required by the build. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* Offset is 1; undefined for n=0 but avoid UB */
    ensure_terms(n);
    return checked_cache[n - 1];
}
