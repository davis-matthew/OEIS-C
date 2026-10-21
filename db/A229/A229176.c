/*
 * A229176  Primes p with nonzero digits such that p + product_of_digits and p
 *          - product_of_digits are both prime.
 * https://oeis.org/A229176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th prime p with all non‑zero digits such that both
 *            p + prod(p) and p – prod(p) are prime (search)
 * Range:     u8 n <= 3, u16 n <= 299, u32 n <= 300, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_product, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/digit_product.h"
#include "../utils/is_prime_big.h"

static seq_t *cache     = NULL;   /* stored terms                      */
static size_t cache_cap = 0;      /* capacity of the cache             */
static size_t cache_len = 0;      /* number of terms stored            */
static idx_t  next_candidate = 3; /* next odd number to test for prime */

static void grow_cache(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Ensure cache contains at least the first n terms. */
    while (cache_len < n)
    {
        /* Find the next prime candidate. */
        idx_t cand = next_candidate;
        while (!is_prime_big((seq_t)cand))
        {
            /* cand is odd (except possibly 3 at start), so we step by 2. */
            cand += 2;
        }

        /* Update pointer to the following odd candidate. */
        next_candidate = cand + 2;

        /* Must have all non‑zero digits: product_of_digits must be non‑zero. */
        seq_t prod = digit_product((seq_t)cand);
        if (!prod) continue;           /* skip if any digit is zero          */

        /* Both p ± prod must be prime. */
        seq_t p    = (seq_t)cand;
        seq_t sum  = p + prod;
        seq_t diff = p > prod ? p - prod : 0; /* diff > 0 for valid primes */

        if (!is_prime_big(sum) || !is_prime_big(diff))
            continue;                  /* not a member of the sequence       */

        /* Term qualifies; store it. */
        grow_cache(cache_len + 1);
        cache[cache_len++] = p;
    }

    return cache[n - 1];
}
