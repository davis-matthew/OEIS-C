/*
 * A158055  a(1)=2, a(n+1) is the smallest prime > n*first digit of a(n).
 * https://oeis.org/A158055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(1) = 2, a(n+1) = the smallest prime strictly > n *
 *            first_digit(a(n))
 * Range:     u8 n <= 31, u16 n <= 8193, u32 n <= 6166, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, abort */
#include <stdbool.h>  /* bool */
#include "../utils/is_prime_big.h"

/* Cache for already computed terms (0‑based: a(1) at cache[0]) */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of terms currently stored */
static size_t cache_cap = 0;   /* allocated capacity */

/* Return the first decimal digit of x (0 ≤ x). */
static seq_t first_digit(seq_t x)
{
    while (x >= (seq_t)10) {
        x = x / (seq_t)10;
    }
    return x;   /* 0‑9 */
}

/* Grow the cache capacity to at least needed elements. */
static void ensure_capacity_size(size_t needed)
{
    if (needed < cache_cap) {
        return;
    }
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap <= needed) {
        new_cap *= 2;
    }
    seq_t *new_cache = (seq_t *)realloc((void *)cache, new_cap * sizeof(seq_t));
    if (!new_cache) {
        abort();  /* allocation failure */
    }
    cache = new_cache;
    cache_cap = new_cap;
}

/* Computes a(n) for any n >= 1, with memoisation. */
seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;   /* undefined for n < 1, defensive return */
    }

    size_t idx = (size_t)n - 1;   /* convert to 0‑based index */

    /* Compute any missing terms up to idx. */
    if (idx >= cache_len) {
        ensure_capacity_size(idx);
        for (size_t i = cache_len; i <= idx; ++i) {
            if (i == 0) {           /* a(1) */
                cache[0] = (seq_t)2;
                continue;
            }

            /* Previous value a(i) (since i is 0‑based, a(i) = cache[i-1]) */
            seq_t prev = cache[i - 1];

            /* First decimal digit of a(i) */
            seq_t fd = first_digit(prev);

            /* Compute n * first_digit, with n = i+1 */
            seq_t prod = ((seq_t)i) * fd;   /* i is 0‑based, matches n-1 */

            /* Search for the smallest prime > prod */
            seq_t candidate = prod + (seq_t)1;
            while (!is_prime_big(candidate)) {
                candidate += (seq_t)1;
            }

            cache[i] = candidate;  /* store a(i+1) */
        }
        cache_len = idx + 1;
    }

    return cache[idx];
}
