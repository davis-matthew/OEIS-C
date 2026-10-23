/*
 * A160180  Largest proper divisor of the n-th composite number.
 * https://oeis.org/A160180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = largest proper divisor of the n‑th composite number (i.e.
 *            A002808(n) / A056608(n)). Computation: find the n‑th composite,
 *            divide it by its smallest prime factor
 * Range:     u8 n <= 413, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>


static seq_t *cache = NULL;          /* cache[i] holds a(i+1) */
static size_t cache_len = 0;         /* number of terms cached */
static size_t cache_cap = 0;         /* current capacity of cache */
static uint64_t current_num = 1;     /* next integer to test (starts at 1) */

/* Return the smallest prime factor of n (n >= 2).  If n is prime
   the function returns n itself. */
static uint64_t spf_of(uint64_t n)
{
    if ((n & 1ULL) == 0) return 2ULL;
    for (uint64_t d = 3ULL; d * d <= n; d += 2ULL)
        if (n % d == 0) return d;
    return n;
}

/* Extend the cache so that it contains at least n terms (1‑based). */
static void ensure_cache(idx_t n)
{
    while (cache_len < (size_t)n)
    {
        /* Find the next composite number. */
        do {
            ++current_num;
        } while (current_num <= 3ULL || spf_of(current_num) == current_num);

        /* Now current_num is composite. Compute its largest proper divisor. */
        uint64_t comp = current_num;
        uint64_t spf  = spf_of(comp);
        seq_t val = (seq_t)(comp / spf);

        /* Grow the cache if necessary. */
        if (cache_len == cache_cap)
        {
            size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();                  /* unrecoverable error */
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len++] = val;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* defensive; not used by OEIS */
    ensure_cache(n);
    return cache[n - 1];
}
