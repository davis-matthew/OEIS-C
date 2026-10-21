/*
 * A240721  Expansion of -(4*x + sqrt(1-8*x) - 1)/(sqrt(1-8*x)*(4*x^2+x) +
 *          8*x^2 - x).
 * https://oeis.org/A240721
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -(n+1)*(3*n-2)*a(n) + (21*n^2-5*n-2)*a(n-1) +
 *            4*(3*n+1)*(2*n-1)*a(n-2) = 0, with a(0) = 1, a(1) = 7
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;        /* array of computed terms */
static idx_t  cache_max = 0;       /* highest index already stored */
static size_t cache_cap = 0;       /* allocated capacity */

seq_t a(idx_t n)
{
    /* Initialise cache on first call */
    if (cache == NULL) {
        /* Allocate at least enough space for n+1, but avoid sending
           zero to malloc.  Use 2 for the trivial small cases. */
        cache_cap = (n >= 1 ? (size_t)(n + 1) : 2);
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return 0;            /* allocation failed */
        cache[0] = (seq_t)1;                    /* a(0) */
        if (cache_cap > 1)
            cache[1] = (seq_t)7;                /* a(1) */
        cache_max = (cache_cap > 1 ? (idx_t)1 : (idx_t)0);
    }

    /* If requested value already computed, just return it */
    if (n <= cache_max) return cache[n];

    /* Ensure capacity for at least n+1 entries */
    if ((size_t)(cache_max + 1) <= n) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;   /* double until enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;                    /* realloc failed */
        cache = tmp;
        cache_cap = new_cap;
    }

#ifdef __cplusplus
    /* C++ implementation – rely on seq_t supporting arbitrary-precision */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t a_im1 = cache[i-1];
        seq_t a_im2 = cache[i-2];

        seq_t coeff1 = (seq_t)((21 * (seq_t)i * (seq_t)i) - (5 * (seq_t)i) - 2);
        seq_t c2     = (seq_t)(4 * ((3 * (seq_t)i + 1)) * ((2 * (seq_t)i - 1)));
        seq_t denom  = (seq_t)((i + 1) * (3 * (seq_t)i - 2));

        seq_t an = (coeff1 * a_im1 + c2 * a_im2) / denom;
        cache[i] = an;
    }
#else
    /* C implementation – use unsigned __int128 for intermediate arithmetic */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t a_im1 = cache[i-1];
        seq_t a_im2 = cache[i-2];

        /* Compute coefficients in 128‑bit */
        unsigned __int128 coeff1 = (unsigned __int128)21 * i * i
                                  - (unsigned __int128)5 * i
                                  - 2;
        unsigned __int128 c2 = (unsigned __int128)4 * (3 * i + 1) * (2 * i - 1);
        unsigned __int128 denom = (unsigned __int128)(i + 1) * (3 * i - 2);

        /* Compute numerator with 128‑bit precision */
        unsigned __int128 term1 = coeff1 * (unsigned __int128)a_im1;
        unsigned __int128 term2 = c2 * (unsigned __int128)a_im2;
        unsigned __int128 num   = term1 + term2;

        /* Division is exact */
        seq_t an = (seq_t)(num / denom);
        cache[i] = an;
    }
#endif

    cache_max = n;
    return cache[n];
}
