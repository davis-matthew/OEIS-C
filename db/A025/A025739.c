/*
 * A025739  Index of 9^n within the sequence of the numbers of the form
 *          9^i*10^j.
 * https://oeis.org/A025739
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + Σ_{k=1..n} ceil(k * log10(9))
 * Range:     u8 n <= 22, u16 n <= 369, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* log10(9) ≈ 0.954242509439324934, represented as a rational
   with a 64‑bit numerator and denominator.  The fraction is accurate
   enough for all k ≤ 10 000 and, because of the integer arithmetic
   below, works for any n that fits in idx_t. */

#include <stdlib.h>          /* for malloc, free */

#define LOG9_NUM   954242509439324934ULL   /* numerator of log10(9) * 10^18 */
#define LOG9_DEN   1000000000000000000ULL   /* denominator 10^18 */

/* Memoised computation of a(n).  The cache grows on demand. */
seq_t a(idx_t n)
{
    /* static cache: cache[i] = a(i) */
    static seq_t *cache      = NULL;
    static idx_t cache_size  = 0;     /* number of allocated entries */
    static idx_t max_computed = 0;    /* highest index computed so far */

    /* Initialise cache for a(0) if not already done. */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        cache[0] = (seq_t)1;               /* a(0) = 1 */
        cache_size = 1;
        max_computed = 0;
    }

    /* Grow cache if necessary. */
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        for (idx_t i = 0; i <= max_computed; ++i)
            new_cache[i] = cache[i];
        free(cache);
        cache = new_cache;
        cache_size = new_size;
    }

    /* Compute missing values up to n. */
    if (n > max_computed) {
        for (idx_t k = max_computed + 1; k <= n; ++k) {
            /* ceil(k * LOG9_NUM / LOG9_DEN) = (k*LOG9_NUM + LOG9_DEN - 1) / LOG9_DEN */
            unsigned __int128 prod   = (unsigned __int128)k * LOG9_NUM;
            unsigned __int128 ceil_k =
                (prod + LOG9_DEN - 1u) / LOG9_DEN;
            cache[k] = cache[k - 1] + (seq_t)ceil_k;
        }
        max_computed = n;
    }

    return cache[n];
}
