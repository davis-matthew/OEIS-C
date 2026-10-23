/*
 * A026392  T(n,[ n/2 ]), where T is the array in A026386.
 * https://oeis.org/A026392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   (n+1)*a(n)+2*(n-1)*a(n-1)+2*(-3*n+1)*a(n-2)+4*(-3*n+7)*a(n-3)+5*(n-3)*a(n-4)+10*(n-5)*a(n-5)=0
 *            Initial values: a(1)=1, a(2)=2, a(3)=4, a(4)=8, a(5)=17
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* cache[0] unused, cache[1..5] hold base values */
static idx_t cache_size = 0;         /* number of elements in cache (== current max index + 1) */

seq_t a(idx_t n)
{
    if (n == 0U) return 0; /* offset starts at 1; n==0 is undefined, return 0 */

    /* Initialise cache with first five terms on first call */
    if (cache_size == 0U) {
        cache = (seq_t *)malloc(6 * sizeof(seq_t));   /* indices 0..5 */
        if (!cache) return 0;                         /* allocation failure fallback */
        cache[1] = 1U;
        cache[2] = 2U;
        cache[3] = 4U;
        cache[4] = 8U;
        cache[5] = 17U;
        cache_size = 6U;                               /* last filled index + 1 */
    }

    /* Return cached value if already computed */
    if (n < cache_size) return cache[n];

    /* Grow cache to accommodate index n */
    idx_t old_size = cache_size;                     /* previous count */
    seq_t *new_cache = (seq_t *)realloc(cache,
                                        (n + 1U) * sizeof(seq_t));
    if (!new_cache) return 0;                         /* allocation failure fallback */
    cache = new_cache;
    /* Initialise new entries to 0 */
    for (idx_t i = old_size; i <= n; ++i) cache[i] = 0U;
    cache_size = n + 1U;

    /* Compute missing terms from old_size up to n */
    for (idx_t i = old_size; i <= n; ++i) {
        if (i <= 5U) continue;                       /* base terms already set */

        /* Fetch the five necessary previous values */
        seq_t a1 = cache[i - 1U];
        seq_t a2 = cache[i - 2U];
        seq_t a3 = cache[i - 3U];
        seq_t a4 = cache[i - 4U];
        seq_t a5 = cache[i - 5U];

        /* Sum of terms with positive coefficients */
        seq_t sum_pos = 0U;
        sum_pos += (seq_t)(2U * (i - 1U)) * a1;      /* 2*(i-1)*a(i-1)   */
        sum_pos += (seq_t)(5U * (i - 3U)) * a4;      /* 5*(i-3)*a(i-4)   */
        sum_pos += (seq_t)(10U * (i - 5U)) * a5;     /* 10*(i-5)*a(i-5)  */

        /* Sum of absolute values of terms with negative coefficients */
        seq_t sum_neg = 0U;
        sum_neg += (seq_t)(2U * (3U * i - 1U)) * a2; /* 2*(3i-1)*a(i-2)  */
        sum_neg += (seq_t)(4U * (3U * i - 7U)) * a3; /* 4*(3i-7)*a(i-3)  */

        /* Numerator = |negative terms| - sum_pos (guaranteed non‑negative) */
        seq_t numer = sum_neg - sum_pos;

        /* Denominator is (i+1) */
        cache[i] = numer / (i + 1U);
    }

    return cache[n];
}
