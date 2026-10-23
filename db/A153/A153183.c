/*
 * A153183  Numbers k such that 3k-2 is prime.
 * https://oeis.org/A153183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) as the n‑th integer k for which 3*k - 2 is prime. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Static cache of previously found k values */
static seq_t *k_cache = NULL;
static size_t k_cap = 0;
static size_t k_len = 0;
static idx_t last_checked = 0;

/* Ensure the cache can hold at least 'new_len' elements. */
static void ensure_capacity(size_t new_len)
{
    if (new_len <= k_cap)
        return;
    size_t new_cap = k_cap ? k_cap << 1 : 32;
    while (new_cap < new_len)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(k_cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort(); /* allocation failed */
    k_cache = tmp;
    k_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for n = 0, return 0 as sentinel */

    /* Grow the cache until we have at least 'n' elements */
    while (k_len < (size_t)n) {
        ++last_checked;
        idx_t candidate = 3 * last_checked - 2;
        if (is_prime(candidate)) {
            ensure_capacity(k_len + 1);
            k_cache[k_len++] = (seq_t)last_checked;
        }
    }
    return k_cache[(size_t)n - 1];
}
