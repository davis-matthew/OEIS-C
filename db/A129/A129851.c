/*
 * A129851  Take the absolute difference between two consecutive terms : the
 *          result shares at least one digit with one of the terms.
 * https://oeis.org/A129851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1; for n > 1, a(n) is the smallest integer > a(n‑1)
   such that |a(n) – a(n‑1)| shares at least one decimal digit with
   either a(n‑1) or a(n). */

#include <stdlib.h>
#include <string.h>

/* Return a bit mask of the decimal digits present in x. */
static inline unsigned digit_mask(seq_t x)
{
    if (x == (seq_t)0) return 1u;     /* digit 0 only */
    unsigned mask = 0u;
    while (x != (seq_t)0) {
        unsigned d = (unsigned)(x % (seq_t)10);
        mask |= 1u << d;
        x /= (seq_t)10;
    }
    return mask;
}

/* --- memoisation ---------------------------------------------------- */
static seq_t *cache     = NULL;   /* cache[1..cache_cap] (index 0 unused) */
static idx_t  cache_cap = 0;      /* largest index currently stored   */
static idx_t  cache_size = 0;     /* allocated array size minus 1     */

/* Grow the cache so it can store indices up to at least n. */
static void ensure_cache(idx_t n)
{
    if (n <= cache_size) return;

    idx_t new_size = (cache_size == 0) ? 16 : cache_size + 1;
    while (new_size < n + 1) new_size <<= 1;   /* ensure space for indices <= n */

    seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
    if (new_cache == NULL) exit(EXIT_FAILURE);

    if (cache) {
        memcpy(new_cache, cache, (cache_cap + 1) * sizeof(seq_t));
        free(cache);
    }
    cache = new_cache;
    cache_size = new_size - 1;
}

/* --------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;   /* undefined for n<1 */

    ensure_cache(n);

    /* First term initialisation */
    if (cache_cap == 0) {
        cache[1] = (seq_t)1;
        cache_cap = 1;
    }

    /* Compute terms up to n if not yet done */
    if (n > cache_cap) {
        for (idx_t i = cache_cap + 1; i <= n; ++i) {
            seq_t prev = cache[i - 1];
            seq_t cand = prev + (seq_t)1;

            while (1) {
                seq_t diff = cand - prev;
                unsigned mask_xy   = digit_mask(prev) | digit_mask(cand);
                unsigned mask_diff = digit_mask(diff);
                if (mask_diff & mask_xy) break;  /* satisfies the digit‑share condition */
                ++cand;
            }
            cache[i] = cand;
        }
        cache_cap = n;
    }

    return cache[n];
}
