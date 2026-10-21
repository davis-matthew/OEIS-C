/*
 * A052758  Expansion of e.g.f.: -(log(1-x))^3*x.
 * https://oeis.org/A052758
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6*n*s(n-1,3) where s(m,k) are unsigned Stirling numbers
 *            of the * first kind. This follows from the e.g.f.
 *            –(log(1‑x))³*x. Each term is * computed using the recurrence *
 *            s(n,k) = s(n-1,k-1) + (n-1)*s(n-1,k) . * Only the values for k =
 *            0…3 are required, giving an O(n) algorithm that * memois...
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* External integer types supplied by the build system */
typedef idx_t idx_t;   /* index type (unsigned 64‑bit) */
typedef seq_t seq_t;   /* value type, unsigned, arbitrary size */

/* Returns a(n) for OEIS A052758.  */
seq_t a(idx_t n)
{
    /* ---- static cache ---- */
    static seq_t *cache          = NULL;   /* array of a(0)…a(current_max)      */
    static size_t cache_size     = 0;       /* capacity of the array             */
    static idx_t cache_max       = 0;       /* largest n for which a(n) is stored */
    /* ---- current Stirling numbers for all k ≤ 3 ---- */
    static seq_t s[4] = {1, 0, 0, 0};       /* s(0,0)=1, others 0                */
    static idx_t s_index = 0;               /* index i for which s holds s(i,k)   */

    /* ---- initialise cache on first call ---- */
    if (cache == NULL) {
        cache_size = 4;                     /* indices 0…3 are guaranteed available */
        cache = (seq_t *)calloc(cache_size, sizeof(seq_t));
        if (!cache) return 0;               /* out of memory: return 0 */
        cache[0] = cache[1] = cache[2] = cache[3] = 0ULL;
        cache_max = 3;                      /* a(0) … a(3) are 0                 */
    }

    /* ---- simple case for small n ---- */
    if (n <= cache_max) return cache[n];

    /* ---- ensure cache has room for a(n) ---- */
    if (cache_size <= n) {                  /* need indices up to n            */
        size_t new_size = cache_size ? cache_size : 4;
        while (new_size <= n) new_size <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;                 /* out of memory: return 0         */
        /* initialise new entries to 0 */
        for (size_t i = cache_size; i < new_size; ++i) tmp[i] = 0;
        cache = tmp;
        cache_size = new_size;
    }

    /* ---- extend computation up to n ---- */
    for (idx_t i = s_index + 1; i <= n - 1; ++i) {
        seq_t new_s[4];
        new_s[0] = 0;                       /* s(i,0) = 0 for i > 0 */
        seq_t mul = (seq_t)(i - 1);         /* (n-1) factor in recurrence */
        for (int k = 1; k <= 3; ++k) {
            /* s(i,k) = s(i-1,k-1) + (i-1)*s(i-1,k) */
            new_s[k] = s[k - 1] + mul * s[k];
        }
        /* copy back */
        for (int k = 0; k <= 3; ++k) s[k] = new_s[k];

        /* compute a(i+1) = 6*(i+1)*s(i,3) */
        cache[i + 1] = (seq_t)6 * ((seq_t)(i + 1)) * s[3];
    }

    /* ---- update state ---- */
    s_index = n - 1;
    cache_max = n;

    return cache[n];
}
