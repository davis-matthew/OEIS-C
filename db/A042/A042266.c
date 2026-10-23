/*
 * A042266  Numerators of continued fraction convergents to sqrt(659).
 * https://oeis.org/A042266
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 13, u64 n <= 26, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A042266 – Numerators of continued‑fraction convergents to sqrt(659) *
 *  Recurrence: a(0)=25, a(1)=26, …, a(11)=70329799
 *  a(n) = 11860*a(n-6) – a(n-12)   for n ≥ 12
 */

#include <stdlib.h>
#include <string.h>

static const seq_t init_vals[12] = {
    25ULL, 26ULL, 77ULL, 1951ULL, 3979ULL, 5930ULL,
    300479ULL, 306409ULL, 913297ULL, 23138834ULL,
    47190965ULL, 70329799ULL
};

static seq_t *cache = NULL;      /* memoised values */
static idx_t cache_len  = 0;      /* allocated size */
static idx_t cache_got  = 0;      /* highest index already computed */

seq_t a(idx_t n)
{
    /* small indices – return constant table */
    if (n < 12) return init_vals[n];

    /* initialise cache on first use or enlarge it when needed */
    if (cache_len < 12) {
        size_t need = 12;
        cache = (seq_t*)malloc(need * sizeof(seq_t));
        if (!cache) return 0;          /* memory failure – unlikely */
        memcpy(cache, init_vals, 12 * sizeof(seq_t));
        cache_len = 12;
        cache_got  = 11;               /* indices 0..11 known */
    }
    if ((idx_t)n + 1 > cache_len) {
        seq_t *new_cache = (seq_t*)realloc(cache, ((n+1)*sizeof(seq_t)));
        if (!new_cache) return 0;      /* memory failure – unlikely */
        cache      = new_cache;
        cache_len  = n + 1;
    }

    /* compute any missing values up to n */
    if (n > cache_got) {
        for (idx_t i = cache_got + 1; i <= n; ++i) {
            /* i ≥ 12 here, so i-6 and i-12 are valid */
            cache[i] = 11860 * cache[i-6] - cache[i-12];
        }
        cache_got = n;
    }
    return cache[n];
}
