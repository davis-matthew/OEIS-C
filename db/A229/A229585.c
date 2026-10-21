/*
 * A229585  Number of defective 3-colorings of an n X 7 0..2 array connected
 *          horizontally and antidiagonally with exactly one mistake, and
 *          colors introduced in row-major 0..2 order.
 * https://oeis.org/A229585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 84*a(n-1) - 2466*a(n-2) + 31428*a(n-3) - 206469*a(n-4) +
 *            750384*a(n-5) - 1513404*a(n-6) + 1574640*a(n-7) - 656100*a(n-8)
 *            Initial values: a(1)=96, a(2)=5724, a(3)=264300, a(4)=11035044,
 *            a(5)=435326724, a(6)=16551428868, a(7)=613195191972,
 *            a(8)=22285439501940
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Offset: first term is a(1).  Return 0 for n==0 (not used). */
    if (n == 0) return 0;

    /* Initial values of the recurrence */
    static const seq_t init_terms[8] = {
        (seq_t)96ULL,
        (seq_t)5724ULL,
        (seq_t)264300ULL,
        (seq_t)11035044ULL,
        (seq_t)435326724ULL,
        (seq_t)16551428868ULL,
        (seq_t)613195191972ULL,
        (seq_t)22285439501940ULL
    };

    /* Recurrence coefficients (absolute values for multiplication) */
    static const long long coeff[8] = {
        84LL,       -2466LL,     31428LL,   -206469LL,
        750384LL,   -1513404LL,  1574640LL, -656100LL
    };

    /* Static cache to store computed terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of terms stored */
    static size_t cache_cap  = 0;   /* allocated capacity */

    /* Initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 8;
        cache_size = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure (unlikely) */
        for (size_t i = 0; i < 8; ++i)
            cache[i] = init_terms[i];
    }

    /* If the requested term is already cached, return it */
    if ((size_t)(n - 1) < cache_size)
        return cache[n - 1];

    /* Extend cache if necessary */
    if ((size_t)n > cache_cap) {
        size_t new_cap = (size_t)n;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return 0;      /* allocation failure */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing terms using the recurrence */
    for (idx_t idx = (idx_t)(cache_size + 1); idx <= n; ++idx) {
        size_t idx0 = (size_t)(idx - 1);          /* zero‑based index of a(idx) */

        seq_t sum_pos = 0;   /* accumulate terms with positive coefficient */
        seq_t sum_neg = 0;   /* accumulate terms with negative coefficient */

        for (size_t k = 1; k <= 8; ++k) {
            seq_t term = cache[idx0 - k];         /* a(idx-k) */
            long long c = coeff[k - 1];
            long long abs_c = (c >= 0) ? c : -c;
            seq_t prod = term * (seq_t)abs_c;     /* term * |c| */

            if (c >= 0) {
                sum_pos += prod;
            } else {
                sum_neg += prod;
            }
        }

        cache[idx0] = sum_pos - sum_neg;          /* since result is positive */
        cache_size = idx0 + 1;
    }

    return cache[n - 1];
}
