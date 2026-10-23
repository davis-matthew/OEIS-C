/*
 * A028575  Row sums of triangle A011801.
 * https://oeis.org/A028575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20*(n-3)*a(n-1) -30*(5*n^2-35*n+62)*a(n-2)
 *            +100*(n-4)*(5*n^2-40*n+81)*a(n-3)
 *            -(5*n-22)*(5*n-21)*(5*n-24)*(5*n-23)*a(n-4) + a(n-5) initial
 *            values (n≥1): a(1)=1, a(2)=5, a(3)=49, a(4)=721, a(5)=14177
 *            (OEIS A028575 – Row sums of triangle A011801)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* 1‑based cache: cache[1] = a(1) */
static size_t cache_len = 0;     /* highest n cached (≥5 after init) */
static size_t cache_alloc = 0;   /* allocated size of cache */

/* Resize cache to accommodate at least 'need' indices (1‑based). */
static void ensure_cache_size(size_t need)
{
    if (cache_alloc >= need) return;
    size_t new_alloc = cache_alloc ? cache_alloc * 2 : 8;
    while (new_alloc < need) new_alloc *= 2;
    seq_t *new_cache = (seq_t *)realloc(cache, (new_alloc + 1) * sizeof(seq_t));
    cache = new_cache;
    cache_alloc = new_alloc;
}

/* Initialize the cache with the first five terms. */
static void init_cache(void)
{
    ensure_cache_size(6);
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)5;
    cache[3] = (seq_t)49;
    cache[4] = (seq_t)721;
    cache[5] = (seq_t)14177;
    cache_len = 5;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n<1, return 0 */

    if (cache == NULL) init_cache();

    if ((size_t)n > cache_len) {
        /* extend cache up to n */
        ensure_cache_size((size_t)n);
        for (size_t k = cache_len + 1; k <= (size_t)n; ++k) {
            /* Fetch previous five values */
            seq_t a1 = cache[k - 1];
            seq_t a2 = cache[k - 2];
            seq_t a3 = cache[k - 3];
            seq_t a4 = cache[k - 4];
            seq_t a5 = cache[k - 5];

            /* Coefficients – computed using seq_t arithmetic */
            seq_t n_seq = (seq_t)k;

            seq_t coeff1 = (seq_t)20 * ((seq_t)(k - 3));
            seq_t coeff2 = (seq_t)30 *
                           ((seq_t)5 * n_seq * n_seq - (seq_t)35 * n_seq + (seq_t)62);
            seq_t coeff3 = (seq_t)100 *
                           ((seq_t)(k - 4)) *
                           ((seq_t)5 * n_seq * n_seq - (seq_t)40 * n_seq + (seq_t)81);
            seq_t coeff4 = ((seq_t)(5 * k - 22)) *
                           ((seq_t)(5 * k - 21)) *
                           ((seq_t)(5 * k - 24)) *
                           ((seq_t)(5 * k - 23));

            seq_t val = coeff1 * a1
                        - coeff2 * a2
                        + coeff3 * a3
                        - coeff4 * a4
                        + a5;

            cache[k] = val;
        }
        cache_len = (size_t)n;
    }

    return cache[n];
}
