/*
 * A042041  Denominators of continued fraction convergents to sqrt(544).
 * https://oeis.org/A042041
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   * a(n) = 4898*a(n-4) - a(n-8) for n ≥ 8 * a(0) = 1, a(1) = 3,
 *            a(2) = 34, a(3) = 105, * a(4) = 4864, a(5) = 14697, a(6) =
 *            166531, a(7) = 514290
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* ---------- Private storage (shared by all calls) ---------- */
static seq_t *cache = NULL;        /* dynamic array of computed values   */
static size_t cache_cap = 0;       /* current allocated capacity        */
static idx_t known_max = 0;        /* highest index for which value is set */

/* ---------- Helper: enlarge the cache to cover indices up to 'up_to' ---------- */
static void ensure_capacity(idx_t up_to)
{
    if (cache_cap > (size_t)up_to)
        return;                               /* already sufficient      */

    size_t new_cap = cache_cap ? cache_cap : 8;
    while (new_cap <= (size_t)up_to)
        new_cap <<= 1;                         /* double until large enough */

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp == NULL)
        exit(EXIT_FAILURE);                   /* out of memory          */
    cache = tmp;
    cache_cap = new_cap;
}

/* ---------- Public function required by the task ---------- */
seq_t a(idx_t n)
{
    /* First‑time initialisation of cache and known base values */
    if (cache == NULL) {
        ensure_capacity(7);                    /* need indices 0 … 7      */

        cache[0] = 1;
        cache[1] = 3;
        cache[2] = 34;
        cache[3] = 105;
        cache[4] = 4864;
        cache[5] = 14697;
        cache[6] = 166531;
        cache[7] = 514290;

        known_max = 7;
    }

    /* Extend cache if required */
    if (n > known_max) {
        ensure_capacity(n);

        /* Compute any missing values using the recurrence */
        for (idx_t i = known_max + 1; i <= n; ++i) {
            cache[i] = (seq_t)4898 * cache[i - 4] - cache[i - 8];
        }
        known_max = n;
    }

    return cache[n];
}
