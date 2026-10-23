/*
 * A154414  Primes of the form 20*k^2 + 32*k + 13.
 * https://oeis.org/A154414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 22, u32 n <= 25, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A154414
 *  a(n) = prime of the form 20*k^2 + 32*k + 13 where k starts from 0
 *  (offset = 1)
 *  The function searches sequentially for primes of the polynomial
 *  and memoises the results for arbitrary access order.
 */

#include <stdlib.h>
#include "../utils/is_prime_big.h"

/* static buffer for caching the sequence values */
static seq_t *cache   = NULL;   /* array of found primes */
static size_t cache_cap = 0;    /* current capacity of cache */
static size_t cache_len = 0;    /* number of primes stored in cache */
static idx_t  next_k   = 0;     /* next k value to test */

/* helper: grow cache if necessary */
static void ensure_cache_capacity(void)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) {
            /* In a production code we would handle out‑of‑memory,
               but for the OEIS build we assume success. */
            exit(1);
        }
        cache = new_cache;
        cache_cap = new_cap;
    }
}

/* helper: generate primes of the form until we have at least n terms */
static void fill_cache_to(idx_t n)
{
    while ((idx_t)cache_len < n) {
        /* compute f(k) = 20*k^2 + 32*k + 13 */
        seq_t k_t = (seq_t)next_k;
        seq_t val = ((seq_t)20 * k_t * k_t) + ((seq_t)32 * k_t) + ((seq_t)13);

        if (is_prime_big(val)) {
            ensure_cache_capacity();
            cache[cache_len++] = val;
        }
        ++next_k;
    }
}

/* --------------  public interface  -------------- */
seq_t a(idx_t n)
{
    if (n < 1) {
        /* behaviour for n < 1 is undefined; return 0 for safety */
        return (seq_t)0;
    }

    fill_cache_to(n);
    return cache[n - 1];
}
