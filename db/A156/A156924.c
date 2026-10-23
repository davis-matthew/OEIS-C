/*
 * A156924  Fifth right hand column (n-m=4) of the A156920 triangle.
 * https://oeis.org/A156924
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   Linear recurrence of order 15 a(n) = 55*a(n-1) - 1365*a(n-2) +
 *            20251*a(n-3) - 200557*a(n-4) + 1402203*a(n-5) - 7137473*a(n-6) +
 *            26886431*a(n-7) - 75433971*a(n-8) + 157376597*a(n-9) -
 *            241846607*a(n-10) + 268663713*a(n-11) - 208880991*a(n-12) +
 *            107416665*a(n-13) - 32730075*a(n-14) + 4465125*a(n-15)...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Initial 15 terms of the sequence */
static const seq_t init_terms[15] = {
    1ULL, 83ULL, 2685ULL, 56285ULL, 919615ULL,
    12813843ULL, 160206627ULL, 1854550395ULL,
    20291056470ULL, 212826091180ULL, 2161547322134ULL,
    21414479565774ULL, 208076662576370ULL,
    1991164206775450ULL, 18825064380813450ULL
};

/* Coefficients for the recurrence split into positive and subtracted parts */
static const int pos_coeff[8] = {
    55, 20251, 1402203, 26886431, 157376597,
    268663713, 107416665, 4465125
};

static const int neg_coeff[7] = {
    1365, 200557, 7137473, 75433971, 241846607,
    208880991, 32730075
};

static seq_t *cache = NULL;      /* dynamic cache for computed terms */
static size_t cache_len = 0;      /* number of terms stored in cache */

/* Initialize cache with the first 15 terms. */
static void init_cache(void)
{
    if (cache) return;
    cache = (seq_t *)malloc(15 * sizeof(seq_t));
    if (!cache) exit(1);
    for (size_t i = 0; i < 15; ++i)
        cache[i] = init_terms[i];
    cache_len = 15;
}

/* Grow cache so that it contains terms up to index n (inclusive). */
static void ensure_upto(idx_t n)
{
    init_cache();

    if ((size_t)n < cache_len) return;

    size_t new_len = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) exit(1);
    cache = tmp;

    /* Compute missing terms using the recurrence. */
    for (size_t i = cache_len; i < new_len; ++i) {
        if (i < 15) {                     /* safety; already set by init_cache */
            cache[i] = init_terms[i];
            continue;
        }

        __int128 pos_sum = 0;
        __int128 neg_sum = 0;

        /* Positive coefficients (a(n-1), a(n-3), …) */
        for (int k = 0; k < 8; ++k) {
            idx_t idx = i - 1 - 2ULL * (idx_t)k;
            pos_sum += (__int128)cache[idx] * pos_coeff[k];
        }

        /* Negative coefficients (a(n-2), a(n-4), …) */
        for (int k = 0; k < 7; ++k) {
            idx_t idx = i - 2 - 2ULL * (idx_t)k;
            neg_sum += (__int128)cache[idx] * neg_coeff[k];
        }

        cache[i] = (seq_t)(pos_sum - neg_sum);
    }

    cache_len = new_len;
}

seq_t a(idx_t n)
{
    ensure_upto(n);
    return cache[(size_t)n];
}
