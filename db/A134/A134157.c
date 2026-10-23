/*
 * A134157  Number of partitions of n into parts that are odd or == +- 4 mod
 *          10.
 * https://oeis.org/A134157
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Number of partitions of n into parts that are odd or == ±4 mod
 *            10
 * Range:     u8 n <= 21, u16 n <= 57, u32 n <= 173, u64 n <= 572,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#if defined(__cplusplus)
# include <new>
#endif

seq_t a(idx_t n)
{
    static seq_t *dp = NULL;   /* memoised values: dp[i] = a(i) */
    static idx_t dp_max = 0;   /* largest n that has been computed */

    /* ---------- Initialisation for first call ---------- */
    if (dp == NULL) {
#if defined(__cplusplus)
        /* C++: allocate zero‑initialised array and construct objects */
        dp = new seq_t[1]();   /* value‑initialised to zero */
#else
        /* C: use calloc to zero‑initialize */
        dp = (seq_t*)calloc(1, sizeof(seq_t));
#endif
        if (dp == NULL) return 0;   /* allocation failure */
        dp[0] = 1;                  /* a(0) = 1 */
        dp_max = 0;
    }

    /* ---------- Extend table if required ---------- */
    if (n > dp_max) {
        idx_t new_max = n;
        seq_t *tmp = NULL;

#if defined(__cplusplus)
        /* Allocate new array, zero‑initialised, then fill */
        tmp = new seq_t[new_max + 1]();
#else
        /* Allocate zero‑initialised array in C */
        tmp = (seq_t*)calloc(new_max + 1, sizeof(seq_t));
#endif
        if (tmp == NULL) return 0;   /* allocation failed */

        /* Recompute all values up to new_max */
        tmp[0] = 1;   /* base case */
        for (idx_t p = 1; p <= new_max; ++p) {
            /* allowed parts: odd or ≡ ±4 (mod 10) */
            if ( (p & 1u) || (p % 10u == 4u) || (p % 10u == 6u) ) {
                for (idx_t i = p; i <= new_max; ++i) {
                    tmp[i] += tmp[i - p];
                }
            }
        }

        /* Free old storage */
#if defined(__cplusplus)
        delete [] dp;
#else
        free(dp);
#endif

        dp = tmp;
        dp_max = new_max;
    }

    return dp[n];
}
