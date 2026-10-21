/*
 * A223584  Number of nX7 0..3 arrays with antidiagonals unimodal.
 * https://oeis.org/A223584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = U(min(n,7))^(max(n,7)-min(n,7)+1) * product{ U(i)^2 ,
 *            i=1..(min(n,7)-1) } with U(z) =
 *            (z^6+15*z^5+115*z^4+405*z^3+964*z^2+660*z+720)/720. For n>6:
 *            a(n) = 1163 * a(n-1)
 * Range:     u8 none, u16 n <= 1, u32 n <= 2, u64 n <= 5, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* U(z) values for 1 <= z <= 7 */
    static const unsigned long long U_raw[8] = {0ULL, 4ULL, 16ULL, 50ULL, 130ULL,
                                                296ULL, 610ULL, 1163ULL};

    if (n <= 0)
        return 0;   /* undefined below the offset; safe fallback */

    /* Handle n <= 7 directly using the closed form */
    if (n <= 7U) {
        idx_t m = n;
        idx_t e = 8U - n;          /* exponent = 8 - n for n <= 7 */
        seq_t prod = 1;

        for (idx_t i = 1; i < m; ++i) {
            seq_t u = (seq_t)U_raw[i];
            prod *= u;   /* U(i)^2 part, performed twice */
            prod *= u;
        }

        seq_t base = (seq_t)U_raw[m];
        seq_t pow = 1;
        for (idx_t k = 0; k < e; ++k)
            pow *= base;   /* base^e */

        return prod * pow;
    }

    /* For n >= 8, use memoized recurrence: a(n) = 1163 * a(n-1) */
    static seq_t *cache = NULL;      /* index 0 unused, cache[1] = a(1) ... */
    static idx_t cached_max = 0;     /* highest n for which cache[n] is valid */

    /* First-time allocation and precomputation of the first 7 values */
    if (cache == NULL) {
        /* Allocate space for indices 0..7 (index 0 unused) */
        cache = (seq_t*)malloc((size_t)8 * sizeof(seq_t));
        if (!cache) return 0;  /* allocation failure */

        /* Compute a(1) .. a(7) using the closed form */
        for (idx_t i = 1; i <= 7U; ++i) {
            idx_t m = i;
            idx_t e = 8U - i;
            seq_t prod = 1;
            for (idx_t j = 1; j < m; ++j) {
                seq_t u = (seq_t)U_raw[j];
                prod *= u;
                prod *= u;
            }
            seq_t base = (seq_t)U_raw[m];
            seq_t pow = 1;
            for (idx_t k = 0; k < e; ++k)
                pow *= base;
            cache[i] = prod * pow;
        }
        cached_max = 7U;
    }

    /* Grow the cache if necessary */
    if (n > cached_max) {
        size_t new_size = (size_t)n + 1;   /* +1 because index n is needed */
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;   /* allocation failure */

        cache = tmp;
        /* Fill in missing terms using the recurrence */
        for (idx_t i = cached_max + 1; i <= n; ++i) {
            cache[i] = (seq_t)1163 * cache[i - 1];
        }
        cached_max = n;
    }

    return cache[n];
}
