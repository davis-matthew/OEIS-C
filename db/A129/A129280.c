/*
 * A129280  a(1)=1. a(n) = the sum of the earlier terms which are <= n.
 * https://oeis.org/A129280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum_{k < n, a(k) <= n} a(k) (OEIS A129280)
 * Range:     u8 n <= 31, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#ifdef __cplusplus
/* use operator new[] and delete[] for C++ builds */
#endif

seq_t a(idx_t n)
{
    /* static cache of computed terms and their prefix sums */
    static seq_t *A    = NULL;   /* A[1..len]   values of the sequence   */
    static seq_t *Cum  = NULL;   /* Cum[i] = a(1)+...+a(i), Cum[0]=0   */
    static idx_t  len  = 0;      /* number of terms currently cached  */
    static idx_t  cap  = 0;      /* allocated capacity of the arrays   */

    if (n == 0) return (seq_t)0; /* out of bounds; not used in OEIS but safe */

    /* extend cache if necessary */
    if (n > len) {
        /* grow capacity if needed */
        if (cap < n) {
            idx_t new_cap = (cap == 0 ? 16 : cap << 1);
            if (new_cap < n) new_cap = n;

            seq_t *newA = NULL;
            seq_t *newC = NULL;

#ifdef __cplusplus
            newA = new seq_t[new_cap + 1];
            newC = new seq_t[new_cap + 1];
#else
            newA = (seq_t *)malloc((new_cap + 1) * sizeof(seq_t));
            newC = (seq_t *)malloc((new_cap + 1) * sizeof(seq_t));
#endif
            if (!newA || !newC) {
                /* memory allocation failed – return 0 as a safe default */
                return (seq_t)0;
            }

            /* copy existing data into the new arrays */
            for (idx_t i = 0; i <= len; ++i) {
                newA[i] = A ? A[i] : (seq_t)0;
                newC[i] = Cum ? Cum[i] : (seq_t)0;
            }

#ifdef __cplusplus
            /* delete old arrays if they were allocated earlier */
            delete[] A;
            delete[] Cum;
#else
            free(A);
            free(Cum);
#endif
            A = newA;
            Cum = newC;
            cap = new_cap;
        }

        /* compute missing terms sequentially up to n */
        for (idx_t m = len + 1; m <= n; ++m) {
            seq_t value = 0;

            if (m == 1) {
                /* base case */
                value = (seq_t)1;
            } else {
                /* binary search for the largest k < m with A[k] <= m */
                idx_t lo = 1, hi = m - 1;
                idx_t last = 0;
                while (lo <= hi) {
                    idx_t mid = lo + ((hi - lo) >> 1);
                    if (A[mid] <= (seq_t)m) {
                        last = mid;
                        lo   = mid + 1;
                    } else {
                        hi   = mid - 1;
                    }
                }
                /* sum of qualifying terms equals Cum[last] */
                value = last > 0 ? Cum[last] : (seq_t)0;
            }

            A[m] = value;
            Cum[m] = Cum[m - 1] + value;
        }

        len = n;   /* update cache size */
    }

    return A[n];
}
