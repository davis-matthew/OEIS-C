/*
 * A118084  Number of partitions of n such that largest part k occurs at most
 *          floor(k/2) times.
 * https://oeis.org/A118084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 43, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = number of integer partitions of n whose largest part k
 * occurs at most ⌊k/2⌋ times.
 * Computation uses the generating‑function identity:
 *   G(x) = Σ_{k≥1} x^k (1 - x^{k⌊k/2⌋}) / Π_{j=1}^k (1 - x^j)
 *        = Σ_{k≥1} Σ_{m=1}^{⌊k/2⌋} p_{k-1}(n - m·k),
 * where p_{k-1}(t) is the number of partitions of t with all parts ≤ k−1.
 * The algorithm computes all p_{k-1} iteratively and accumulates the
 * contributions to the sequence on the fly.  It caches results up to the
 * largest n requested so that subsequent calls are O(1).
 *
 * The implementation is fully compliant with C11 and C++17 (including
 * arbitrary‑precision seq_t types).
 */

#include <stdlib.h>
#include <stdint.h>

extern seq_t a(idx_t n);

seq_t a(idx_t n)
{
    /* Return 0 for non‑positive indices (should not happen). */
    if (n == 0) return 0;

    /* static cache of computed values */
    static idx_t allocated = 0;          /* highest n for which arrays exist   */
    static seq_t *part = NULL;          /* partitions with parts ≤ k‑1         */
    static seq_t *ans  = NULL;          /* final sequence values               */

    if (n > allocated) {
        /* Allocate new arrays of size n+1 (indices 0..n). */
        size_t sz = (size_t)n + 1;
        seq_t *p = NULL;
        seq_t *a = NULL;

#ifdef __cplusplus
        p = new seq_t[sz];
        a = new seq_t[sz];
#else
        p = (seq_t *)malloc(sz * sizeof(seq_t));
        a = (seq_t *)malloc(sz * sizeof(seq_t));
#endif
        if (!p || !a) { /* allocation failure; return 0 as fallback */
            return 0;
        }

        /* Initialise to zero */
        for (size_t i = 0; i < sz; ++i) {
            p[i] = 0;
            a[i] = 0;
        }

        /* base case: partition of 0 is 1 */
        p[0] = 1;

        /* Main loop: iterate over largest part k */
        for (idx_t k = 1; k <= n; ++k) {
            idx_t max_m = k >> 1;               /* floor(k/2)  */

            for (idx_t m = 1; m <= max_m; ++m) {
                idx_t offset = m * k;          /* m copies of part k */

                /* add contributions to a[sum] for all sums ≥ offset */
                for (idx_t sum = offset; sum <= n; ++sum) {
                    a[sum] += p[sum - offset];
                }
            }

            /* update partition counts to include part k */
            for (idx_t sum = k; sum <= n; ++sum) {
                p[sum] += p[sum - k];
            }
        }

        /* Free old arrays if any */
#ifdef __cplusplus
        delete[] part;
        delete[] ans;
#else
        free(part);
        free(ans);
#endif

        part = p;
        ans  = a;
        allocated = n;
    }

    return ans[n];
}
