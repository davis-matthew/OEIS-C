/*
 * A126189  Number of hex trees with n edges and no adjacent vertices of
 *          outdegree 2.
 * https://oeis.org/A126189
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   (n+4)*a(n) +3*(-2*n-5)*a(n-1) +9*(n+1)*a(n-2)
 *            +6*(-2*n+1)*a(n-3)=0 with a(0)=1, a(1)=3, a(2)=10, a(3)=36. This
 *            recurrence holds for all n≥3
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#ifdef __cplusplus
#  include <vector>
#endif

/* The function is memoised.  For C++ builds a std::vector is used, otherwise
   a static array of size 10001 is allocated (enough for n up to 10000). */
seq_t a(idx_t n) {
#ifndef __cplusplus
    /* Static array for C builds. */
    static seq_t cache[10001];
    /* up_to contains the first index that is not yet computed. */
    static idx_t up_to = 0;
#else
    /* std::vector for C++ builds.  It automatically grows as needed. */
    static std::vector<seq_t> cache;
    static idx_t up_to = 0;
#endif

    /* Initialise for the first call. */
    if (up_to == 0) {
#ifdef __cplusplus
        cache.resize(4);
#endif
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)10;
        cache[3] = (seq_t)36;
        up_to = 4;     /* indices 0..3 are now valid */
    }

    /* If the requested value has already been computed, return it. */
    if (n < up_to) {
        return cache[n];
    }

    /* Compute terms up to n.  The recurrence is
       (n+4)*a(n) = (6*n+15)*a(n-1) - (9*(n+1))*a(n-2) + (12*n-6)*a(n-3) */
    for (idx_t k = up_to; k <= n; ++k) {
        seq_t term1 = ((seq_t)(6 * k + 15)) * cache[k - 1];
        seq_t term3 = ((seq_t)(12 * k - 6)) * cache[k - 3];
        seq_t num    = term1 + term3;
        num -= ((seq_t)(9 * (k + 1))) * cache[k - 2];
        seq_t den = k + 4;
        seq_t val = num / den;   /* division is exact */

#ifdef __cplusplus
        cache.push_back(val);
#else
        cache[k] = val;
#endif
    }
    up_to = n + 1;  /* we now have terms up to index n */
    return cache[n];
}
