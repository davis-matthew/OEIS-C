/*
 * A082455  a(n) = prime(n) + prime(n-1) - a(n-1), a(0) = 0, a(1) = 4.
 * https://oeis.org/A082455
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A082455
   Formula:  a(0) = 0
             a(1) = 4
             a(n) = prime(n) + prime(n-1) – a(n-1)     for n ≥ 2
   The n‑th prime (prime(1)=2, prime(2)=3, …) is obtained from the
   supplied helper get_prime(idx_t); it is cast to seq_t when used.
*/
#include <stdlib.h>
#include "../utils/get_prime.h"   /* provides uint32_t get_prime(idx_t) */

/* ------------------------------------------------------------ */
/*  Cache for previously computed terms                          */
/* ------------------------------------------------------------ */
static seq_t *cache = NULL;     /* cache[0] = 0, cache[1] = 4, … */
static size_t cache_cap = 0;    /* allocated capacity of cache array  */
static size_t cache_len = 0;    /* number of terms currently stored  */

/* Ensure cache can address index n (n >= 0).               */
static void
ensure_cache(idx_t n)
{
    if (cache_cap > (size_t)n)
        return;

    size_t new_cap = cache_cap == 0 ? 16 : cache_cap;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        return;          /* abort on out‑of‑memory – caller will get 0 */
    cache = tmp;
    cache_cap = new_cap;
}

/* ------------------------------------------------------------ */
/*   Main public routine                                        */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    if (n == 1)
        return 4;

    ensure_cache(n);

    /* Initialise the first two terms if the cache is empty. */
    if (cache_len < 2) {
        cache[0] = 0;
        cache[1] = 4;
        cache_len = 2;
    }

    /* Compute any missing values up to n. */
    if (cache_len <= (size_t)n) {
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t pi   = (seq_t)get_prime(i);     /* prime(i)  */
            seq_t pim1 = (seq_t)get_prime(i-1);   /* prime(i-1) */
            cache[i] = pi + pim1 - cache[i-1];
        }
        cache_len = n + 1;
    }

    return cache[n];
}
