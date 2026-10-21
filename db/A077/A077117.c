/*
 * A077117  Least k such that Z(k,6) <= Z(n,7) where Z(m,s) = Sum_{i>=m}
 *          1/i^s.
 * https://oeis.org/A077117
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k such that Z(k,6) <= Z(n,7), * where Z(m,s) =
 *            sum_{i=m}^∞ 1/i^s. * (In practice Z(m,s) is computed for m ≥ 0
 *            with Z(0,s)=Z(1,s).)
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static double *Z6 = NULL;
static double *Z7 = NULL;
static int initialized = 0;
static const idx_t MAX_K = 200000; /* sufficient for n up to ~10000 */

seq_t a(idx_t n)
{
    /* lazy global initialization */
    if (!initialized) {
        const size_t cap = (size_t)MAX_K + 2; /* we use indices 0..MAX_K+1 */
        Z6 = (double *)malloc(cap * sizeof(double));
        Z7 = (double *)malloc(cap * sizeof(double));
        if (!Z6 || !Z7) {
            /* abort on allocation failure */
            exit(1);
        }
        Z6[MAX_K + 1] = 0.0;
        Z7[MAX_K + 1] = 0.0;
        for (idx_t i = MAX_K; i >= 1; --i) {
            /* compute i^6 and i^7 */
            double p6 = 1.0, p7 = 1.0;
            for (int e = 0; e < 6; ++e) p6 *= (double)i;
            for (int e = 0; e < 7; ++e) p7 *= (double)i;
            Z6[(size_t)i] = Z6[(size_t)i + 1] + 1.0 / p6;
            Z7[(size_t)i] = Z7[(size_t)i + 1] + 1.0 / p7;
        }
        /* define Z(0,s) = Z(1,s) */
        Z6[0] = Z6[1];
        Z7[0] = Z7[1];
        initialized = 1;
    }

    /* target value T = Z(n,7)  (for n=0 we get Z0,7 = Z1,7) */
    double T = Z7[(size_t)n];
    /* binary search for the smallest k with Z6[k] <= T */
    size_t lo = 1, hi = (size_t)MAX_K, ans = (size_t)MAX_K;
    while (lo <= hi) {
        size_t mid = lo + ((hi - lo) >> 1);
        if (Z6[mid] <= T) {
            ans = mid;
            if (mid == 0) break; /* cannot go lower */
            hi = mid - 1;
        } else {
            lo = mid + 1;
        }
    }
    return (seq_t)ans;
}
