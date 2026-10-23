/*
 * A041092  Numerators of continued fraction convergents to sqrt(54).
 * https://oeis.org/A041092
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 38, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A041092: numerators of convergents to sqrt(54)
 * Recurrence derived from the continued fraction of sqrt(54).
 * Let a0 = 7, a1 = 15.
 * For n >= 2:   a(n) = A[n] * a(n-1) + a(n-2)
 * where A[n] is the nth term of the periodic continued‑fraction
 * coefficients after the initial integer part:
 *   A = {2, 1, 6, 1, 2, 14} repeating every 6 terms.
 * For n=1, the same formula applies using a(-1)=1:
 *   a(1) = 2*a(0) + 1 = 15.
 */

#include <stdlib.h>

static const idx_t period[6] = {2, 1, 6, 1, 2, 14};

/* Memoisation cache */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_len = 0;

/* Ensure the cache can store values up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return;            /* allocation failed – caller gets 0 */
    }
    if (cache_cap <= n) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return;              /* allocation failed – caller gets 0 */
        cache = tmp;
        cache_cap = new_cap;
    }
}

extern seq_t a(idx_t n)
{
    ensure_cache(n);
    if (!cache) return 0;          /* allocation failed */

    /* Initialise first term */
    if (cache_len == 0) {
        cache[0] = (seq_t)7;
        cache_len = 1;
    }

    while (cache_len <= n) {
        /* Compute next term */
        if (cache_len == 1) {
            /* a(1) = 2*a(0) + 1 */
            cache[1] = (seq_t)period[0] * cache[0] + (seq_t)1;
        } else {
            idx_t idx = cache_len;              /* current n */
            seq_t last = cache[idx - 1];
            seq_t second_last = cache[idx - 2];
            seq_t coeff = (seq_t)period[(idx - 1) % 6];
            cache[idx] = coeff * last + second_last;
        }
        cache_len++;
    }

    return cache[n];
}
