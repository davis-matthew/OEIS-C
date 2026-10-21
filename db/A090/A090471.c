/*
 * A090471  Smallest prime not already used that is of the form prime(n)+kn
 *          for some k > 0.
 * https://oeis.org/A090471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest unused prime of the form prime(n) + k*n, k>0
 * Range:     u8 n <= 22, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A090471 – grows lazily and is cache‑aware. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"

/* Cache of computed a(n) values (0‑based).  cached_n holds the last
   index that was cached (1‑based). */
static idx_t   cached_n = 0;
static seq_t*  cache    = NULL;
static size_t  cache_cap= 0;

/* List of primes that have already appeared in the sequence.
   All comparisons are done via linear search – sufficient for
   n up to 10⁴.  Each element is a prime that has been output. */
static seq_t*  used     = NULL;
static size_t  used_len = 0;
static size_t  used_cap = 0;

/* Resize an array of size 'elem_size' (in bytes) up to at least
   'need' elements, using power‑of‑two growth. */
static void ensure_capacity(size_t need, size_t *cap, void **ptr, size_t elem_size)
{
    if (*cap < need) {
        size_t new_cap = (*cap > 0) ? (*cap << 1) : 16;
        if (new_cap < need) new_cap = need;
        *ptr = realloc(*ptr, new_cap * elem_size);
        *cap = new_cap;
    }
}

/* Linear membership test.  For big seq_t this still works
   because the == operator is defined. */
static bool used_contains(seq_t val)
{
    for (size_t i = 0; i < used_len; ++i)
        if (used[i] == val) return true;
    return false;
}

static void used_add(seq_t val)
{
    if (used_cap == used_len)
        ensure_capacity(used_len + 1, &used_cap, (void**)&used, sizeof(seq_t));
    used[used_len++] = val;
}

/* Compute the nth term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    /* Extend the cache if necessary. */
    if (n > cached_n) {
        /* Allocate space for new terms. */
        ensure_capacity(n, &cache_cap, (void**)&cache, sizeof(seq_t));

        for (idx_t i = cached_n + 1; i <= n; ++i) {
            uint32_t p_i = get_prime(i);                /* i‑th prime, i is 1‑based */

            /* Search for the smallest prime not yet used. */
            idx_t k = 1;
            while (1) {
                seq_t candidate = (seq_t)p_i + ((seq_t)k * (seq_t)i);
                if (is_prime_big(candidate) && !used_contains(candidate)) {
                    used_add(candidate);
                    cache[i - 1] = candidate;           /* cache is 0‑based */
                    break;
                }
                ++k;
            }
        }
        cached_n = n;
    }
    return cache[n - 1];
}
