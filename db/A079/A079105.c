/*
 * A079105  Number of permutations of length n, in which all monotone
 *          subsequences of length 4 are descending or all such subsequences
 *          are ascending, containing the minimum number of such subsequences
 *          subject to that constraint.
 * https://oeis.org/A079105
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f. = (1 + x*(432*x^16 +864*x^15 +432*x^14 +2710*x^13
 *            +4124*x^12 + * 82*x^11 -5128*x^10 -7856*x^9 -1458*x^8 -1678*x^7
 *            -860*x^6 * -300*x^5 -84*x^4 -21*x^3 -6*x^2 -2*x -1)) / * (x^3 -
 *            1) * Recurrence derived from (x^3-1)G(x)=1+xP(x): * a(n) =
 *            a(n-3) – coeff_N(n) * where coeff_N(0)=1 and, for 1≤n≤1...
 * Range:     u8 n <= 5, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static const int64_t coeffP[17] = {
    -1, -2, -6, -21, -84, -300, -860, -1678, -1458, -7856,
    -5128, 82, 4124, 2710, 432, 864, 432
};

static seq_t *cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n) {
    static const seq_t base[6] = {1, 1, 2, 6, 22, 86};

    if (n < 6) return base[n];

    /* Initialize cache if first call */
    if (!cache) {
        cache_size = 6;                     /* indices 0..5 are stored */
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_size);
        if (!cache) return 0;               /* allocation failed */
        for (idx_t i = 0; i < 6; ++i) cache[i] = base[i];
    }

    /* Extend cache if required */
    if (n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_size);
        if (!tmp) return 0; /* allocation failed */
        cache = tmp;

        for (idx_t k = cache_size; k <= n; ++k) {
            int64_t cp = 0;
            if (k <= 17) cp = coeffP[k - 1]; /* coeff_N(k) for k>=1, else 1 for k=0 */
            seq_t prev = cache[k - 3];
            if (cp >= 0)
                cache[k] = prev - (seq_t)cp;
            else
                cache[k] = prev + (seq_t)(-cp);
        }
        cache_size = new_size;
    }

    return cache[n];
}
