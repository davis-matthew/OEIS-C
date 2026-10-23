/*
 * A167951  Number of reduced words of length n in Coxeter group on 35
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^16 = I.
 * https://oeis.org/A167951
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 34 * a(n-1) - 594 * a(n-16) + 561 * a(n-17), plus an
 *            inhomogeneous term: b(0) = b(1) = 1, b(16) = b(17) = -1, b(n) =
 *            0 otherwise. Initial terms (n=0..15) are the values listed in
 *            OEIS
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_from_str.h"   /* seq_from_str(seq_t) */

static const char *const init_str[16] = {
    "1", "35", "1190", "40460", "1375640",
    "46771760", "1590239840", "54068154560",
    "1838317255040", "62502786671360",
    "2125094746826240", "72253221392092160",
    "2456609527331133440", "83524723929258536960",
    "2839840613594790256640", "96554580862222868725760"
};

static seq_t init_val[16];
static bool init_done = false;

/* Lazy initialization of the 16 initial terms. */
static void init_initial_terms(void)
{
    if (!init_done) {
        for (int i = 0; i < 16; ++i)
            init_val[i] = seq_from_str(init_str[i]);
        init_done = true;
    }
}

/* Cached sequence. */
static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* Ensure that the cache contains values up to index n. */
static void ensure_cache(idx_t n)
{
    init_initial_terms();

    /* Allocate the cache if it does not exist yet. */
    if (cache == NULL) {
        cache = (seq_t *)realloc(NULL, 16 * sizeof(seq_t));
        if (cache == NULL) abort();
        for (int i = 0; i < 16; ++i)
            cache[i] = init_val[i];
        cache_size = 16;
    }

    /* Grow the cache if needed. */
    if (n + 1 > cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;

        /* Compute new terms using the recurrence. */
        for (idx_t k = cache_size; k <= n; ++k) {
            if (k < 16) continue;          /* already initialized */

            /* 34 * a(k-1) */
            seq_t term = cache[k - 1] * (seq_t)34;

            /* - 594 * a(k-16) */
            term -= cache[k - 16] * (seq_t)594;

            /* + 561 * a(k-17)  (only if k >= 17) */
            if (k >= 17) {
                term += cache[k - 17] * (seq_t)561;
            }

            /* Inhomogeneous correction for k = 16, 17 */
            if (k == 16 || k == 17) {
                term -= (seq_t)1;
            }

            cache[k] = term;
        }

        cache_size = new_size;
    }
}

/* Public function. */
seq_t a(idx_t n)
{
    if (n < cache_size)
        return cache[n];
    ensure_cache(n);
    return cache[n];
}
