/*
 * A203643  Number of arrays of n 0..12 integers with new values introduced in
 *          order 0..12 but otherwise unconstrained.
 * https://oeis.org/A203643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 25, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 79*a(n-1) - 2783*a(n-2) + 57695*a(n-3) - 782133*a(n-4)
 *            + 7284057*a(n-5) - 47627789*a(n-6) + 219409685*a(n-7)
 *            - 703202566*a(n-8) + 1519272964*a(n-9) - 2082477528*a(n-10)
 *            + 1606986720*a(n-11) - 518918400*a(n-12)
 * Initial values a(1)..a(12) are given in the OEIS entry.
 *
 * This implementation uses an explicit recurrence with static caching.
 * Intermediate products are computed in the widest integer type that
 * guarantees no overflow when the result fits into seq_t.
 * If seq_t is a small builtin integer type (≤ 64 bit) the product is
 * performed in `unsigned __int128` (or `unsigned long long` when
 * `__int128` is not available). For larger seq_t sizes the product
 * uses seq_t directly, which works for arbitrary‑precision implementations.
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial terms a(1) .. a(12) */
    static const seq_t init[12] = {
        (seq_t)1,
        (seq_t)2,
        (seq_t)5,
        (seq_t)15,
        (seq_t)52,
        (seq_t)203,
        (seq_t)877,
        (seq_t)4140,
        (seq_t)21147,
        (seq_t)115975,
        (seq_t)678570,
        (seq_t)4213597
    };

    /* Recurrence coefficients */
    static const int64_t coeff[12] = {
        79, -2783, 57695, -782133,
        7284057, -47627789, 219409685, -703202566,
        1519272964, -2082477528, 1606986720, -518918400
    };

    /* Cache for computed values */
    static seq_t *cache = NULL;
    static size_t cap = 0;       /* allocated size */
    static idx_t  computed = 0;  /* highest n cached */

    /* For small n we can simply return the initial value */
    if (n <= 12)
        return init[n - 1];

    /* Initialise cache on first call */
    if (cache == NULL) {
        /* Allocate enough space for up to n */
        cap = (size_t)n;
        cache = (seq_t *)calloc(cap + 1, sizeof(seq_t));
        for (int i = 0; i < 12; ++i)
            cache[i + 1] = init[i];
        computed = 12;
    }

    /* Grow the cache if needed */
    if (cap < (size_t)n) {
        cap = (size_t)n;
        cache = (seq_t *)realloc(cache, (cap + 1) * sizeof(seq_t));
    }

    /* Compute any missing terms */
    if (computed < n) {
        for (idx_t i = computed + 1; i <= n; ++i) {
            seq_t pos = 0;
            seq_t neg = 0;
            for (int j = 0; j < 12; ++j) {
                idx_t k = i - 1 - j;            /* previous index (>=1) */
                seq_t prod;
                int64_t c = coeff[j];
                int64_t ac = c > 0 ? c : -c;

                /* Choose the widest integer type for the product */
                if (sizeof(seq_t) <= 8) {
#if defined(__SIZEOF_INT128__)
                    unsigned __int128 mult = (unsigned __int128)cache[k] *
                                            (unsigned __int128)ac;
                    prod = (seq_t)mult;
#else
                    unsigned long long mult = (unsigned long long)cache[k] *
                                              (unsigned long long)ac;
                    prod = (seq_t)mult;
#endif
                } else {
                    /* For large seq_t types (big int) rely on native multiplication */
                    prod = cache[k] * (seq_t)ac;
                }

                if (c > 0)
                    pos += prod;
                else
                    neg += prod;
            }
            /* The recurrence guarantees a positive result */
            cache[i] = pos - neg;
        }
        computed = n;
    }

    return cache[n];
}
