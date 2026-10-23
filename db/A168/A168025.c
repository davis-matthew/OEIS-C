/*
 * A168025  Noncomposite numbers in the western ray of the Ulam spiral as
 *          oriented on the March 1964 cover of Scientific American.
 * https://oeis.org/A168025
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*k^2 − 7*k + 4, where k is the (n+1)-th positive integer
 *            for which 4*k^2 − 7*k + 4 is 1 or a prime. The sequence is
 *            generated in order of increasing k
 * Range:     u8 n <= 2, u16 n <= 17, u32 n <= 18, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;      /* cached sequence values */
static size_t cache_len = 0;     /* number of values stored */
static size_t cache_cap = 0;     /* capacity of cache */
static idx_t next_k = 1;         /* next integer k to test */

/* Ensure cache can hold at least "need" elements. */
static void grow_cache(size_t need)
{
    if (cache_cap >= need) return;
    size_t new_cap = (cache_cap == 0) ? 8 : cache_cap;
    while (new_cap < need) new_cap = new_cap * 2 + 8;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the nth term (0‑based) of the sequence. */
seq_t a(idx_t n)
{
    /* Generate terms until we have at least n+1 of them. */
    while (cache_len <= (size_t)n) {
        seq_t k = (seq_t)next_k;
        /* Compute 4*k^2 − 7*k + 4.  All operations are performed in
           seq_t; the sequence values fit into seq_t for all n up
           to the limits required by this problem. */
        seq_t val = k * ((k * (seq_t)4) - (seq_t)7) + (seq_t)4;

        /* Keep the value if it is 1 or a prime. */
        if (val == (seq_t)1 || is_prime_big(val)) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = val;
        }
        ++next_k;
    }
    return cache[(size_t)n];
}
