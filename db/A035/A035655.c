/*
 * A035655  Number of partitions of n into parts 7k and 7k+5 with at least one
 *          part of each type.
 * https://oeis.org/A035655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in (∏_{k≥1} 1/(1 - x^{7k}) - 1) *
 *            (∏_{k≥0} 1/(1 - x^{7k+5}) - 1) a(n) counts partitions of n into
 *            parts 7k and 7k+5 with at least one part of each type
 * Range:     u8 n <= 79, u16 n <= 187, u32 n <= 512, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#ifdef __cplusplus
#define ALLOC_SEQ_T(ptr, sz) ((ptr) = new seq_t[(sz)])
#define FREE_SEQ_T(ptr)      delete[] ptr
#else
#define ALLOC_SEQ_T(ptr, sz) ((ptr) = (seq_t *)malloc((sz) * sizeof(seq_t)))
#define FREE_SEQ_T(ptr)      free(ptr)
#endif

seq_t a(idx_t n)
{
    static seq_t *dp   = NULL, *dpS = NULL, *dpT = NULL;
    static idx_t max_n = 0;

    if (n == 0) return 0;            /* offset starts at 1 */

    if (n > max_n) {
        size_t sz = (size_t)n + 1;

        /* Reallocate and initialise */
        FREE_SEQ_T(dp);
        ALLOC_SEQ_T(dp, sz);
        FREE_SEQ_T(dpS);
        ALLOC_SEQ_T(dpS, sz);
        FREE_SEQ_T(dpT);
        ALLOC_SEQ_T(dpT, sz);

        for (size_t i = 0; i < sz; ++i) {
            dp[i]   = 0;
            dpS[i]  = 0;
            dpT[i]  = 0;
        }
        dp[0] = 1;
        dpS[0] = 1;
        dpT[0] = 1;

        /* Partitions using only multiples of 7 (S) */
        for (idx_t p = 7; p <= n; p += 7) {
            for (idx_t j = p; j <= n; j++) {
                dpS[j] = dpS[j] + dpS[j - p];
            }
        }

        /* Partitions using only numbers ≡5 (mod 7) (T) */
        for (idx_t p = 5; p <= n; p += 7) {
            for (idx_t j = p; j <= n; j++) {
                dpT[j] = dpT[j] + dpT[j - p];
            }
        }

        /* Partitions using parts from both sets (S ∪ T) */
        for (idx_t p = 5; p <= n; p += 7) {
            for (idx_t j = p; j <= n; j++) {
                dp[j] = dp[j] + dp[j - p];
            }
        }
        for (idx_t p = 7; p <= n; p += 7) {
            for (idx_t j = p; j <= n; j++) {
                dp[j] = dp[j] + dp[j - p];
            }
        }

        max_n = n;
    }

    return dp[n] - dpS[n] - dpT[n];
}
