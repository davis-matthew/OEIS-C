/*
 * A142683  Primes congruent to 29 mod 57.
 * https://oeis.org/A142683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes p such that p ≡ 29 (mod 57). a(n) is the n‑th such prime
 * Range:     u8 n <= 1, u16 n <= 177, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 59 terms through n = 999 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

static seq_t *cache = NULL;         /* stored primes */
static size_t cache_len = 0;        /* number of primes stored */
static size_t cache_cap = 0;        /* capacity of the cache */
static idx_t last_k = 0;            /* index k (for candidate formula 57*k+29) */

seq_t a(idx_t n)
{
    /* Ensure we have at least n primes in cache */
    while (cache_len < (size_t)n) {
        idx_t candidate = 57 * last_k + 29;   /* candidate of the form 57k+29 */
        if (is_prime(candidate)) {
            /* append_candidate will grow the array if needed */
            append_candidate(&cache, &cache_len, &cache_cap,
                             (seq_t)candidate);
        }
        ++last_k;
    }
    return cache[n - 1];
}
