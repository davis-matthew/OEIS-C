/*
 * A008129  Coordination sequence T1 for Zeolite Code LIO.
 * https://oeis.org/A008129
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 177, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A008129.  The coefficients of the denominator of the
   generating function give the linear recurrence of order 19

   a(n) = 2*a(n‑1) – a(n‑2) – a(n‑3) + 2*a(n‑4) – 3*a(n‑6) + 3*a(n‑7)
          – 3*a(n‑9) + 3*a(n‑10) – 3*a(n‑12) + 3*a(n‑13) – 2*a(n‑15)
          + a(n‑16) + a(n‑17) – 2*a(n‑18) + a(n‑19)     for n ≥ 20

   The first 20 terms (a(0) … a(19)) are hard‑coded. */
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

static const seq_t base_vals[20] = {
    1, 4, 10, 20, 34, 53, 76, 103, 135, 170,
    209, 252, 300, 354, 410, 470, 534, 602, 676, 752
};

static const int coeffs[19] = {
    2, -1, -1, 2, 0, -3, 3, 0, -3, 3, 0, -3, 3, 0, -2, 1, 1, -2, 1
};

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

static void init_cache(void)
{
    cache_cap = 20;
    cache_len = 20;
    cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    memcpy(cache, base_vals, 20 * sizeof(seq_t));
}

static void ensure_capacity(idx_t n)
{
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n + 1)
            new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n < 20)
        return base_vals[n];

    if (cache == NULL)
        init_cache();

    ensure_capacity(n);

    if ((size_t)n >= cache_len) {
        for (idx_t k = (idx_t)cache_len; k <= n; ++k) {
            __int128 sum = 0;
            for (int i = 0; i < 19; ++i) {
                idx_t idx = k - (idx_t)(i + 1);
                sum += (__int128)coeffs[i] * (__int128)cache[idx];
            }
            cache[k] = (seq_t)sum;
        }
        cache_len = (size_t)n + 1;
    }

    return cache[n];
}
