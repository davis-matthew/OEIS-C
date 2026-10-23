/*
 * A039851  Number of partitions satisfying cn(0,5) + cn(2,5) <= 1 and cn(0,5)
 *          + cn(3,5) <= 1.
 * https://oeis.org/A039851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
    A039851 – Number of partitions satisfying cn(0,5)+cn(2,5) ≤ 1
    and cn(0,5)+cn(3,5) ≤ 1.  This implementation uses dynamic programming:
    - base[n] counts partitions using only parts ≡ 1 or 4 (mod 5),
    - dp1[n] counts partitions with c0 = 0, allowing at most one part
      ≡ 2 (mod 5) and at most one part ≡ 3 (mod 5),
    - dp2[n] counts partitions with exactly one part ≡ 0 (mod 5) and no
      parts ≡ 2 or 3.  The answer is dp1[n] + dp2[n].
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle indices before the sequence offset – behaviour is irrelevant. */
    if (n == 0) return 0;

    /* Static memoisation state. */
    static idx_t     max_n = 0;   /* maximum n already computed */
    static seq_t   *base  = NULL; /* partitions using only 1- or 4‑mod‑5 parts */
    static seq_t   *dp1   = NULL; /* partitions with c0 = 0, ≤1 of 2,3 */
    static seq_t   *dp2   = NULL; /* partitions with c0 = 1, no 2 or 3 */

    /* Ensure we have computed values up to n. */
    if (max_n < n) {
        size_t needed = (size_t)(n + 1);

        /* Allocate new arrays. */
        seq_t *new_base = (seq_t*)malloc(needed * sizeof(seq_t));
        seq_t *new_dp1  = (seq_t*)malloc(needed * sizeof(seq_t));
        seq_t *new_dp2  = (seq_t*)malloc(needed * sizeof(seq_t));

        /* ------------------------------------------------------------------
           Compute base[n]: partitions using only parts ≡ 1 or 4 (mod 5).
         ------------------------------------------------------------------ */
        for (size_t i = 0; i < needed; ++i) new_base[i] = 0;
        new_base[0] = 1;                       /* empty partition */

        /* parts ≡ 1 (mod 5): 1,6,11,… */
        for (idx_t s = 1; s <= n; s += 5) {
            for (idx_t k = s; k <= n; ++k) {
                new_base[(size_t)k] += new_base[(size_t)(k - s)];
            }
        }
        /* parts ≡ 4 (mod 5): 4,9,14,… */
        for (idx_t s = 4; s <= n; s += 5) {
            for (idx_t k = s; k <= n; ++k) {
                new_base[(size_t)k] += new_base[(size_t)(k - s)];
            }
        }

        /* ------------------------------------------------------------------
           Compute dp1[n]: partitions with no 0‑mod‑5 parts, at most one 2‑mod‑5
           and at most one 3‑mod‑5 part.
         ------------------------------------------------------------------ */
        /* Start with partitions using only 1‑ or 4‑mod‑5 parts. */
        for (size_t i = 0; i < needed; ++i) new_dp1[i] = new_base[i];

        /* Add a single part ≡ 2 (mod 5). */
        for (idx_t s = 2; s <= n; s += 5) {
            for (idx_t k = s; k <= n; ++k) {
                new_dp1[(size_t)k] += new_base[(size_t)(k - s)];
            }
        }

        /* Snapshot after adding the 2‑mod‑5 part.  Use a temporary array to
           avoid contamination when we add the 3‑mod‑5 part. */
        seq_t *tmp = (seq_t*)malloc(needed * sizeof(seq_t));
        for (size_t i = 0; i < needed; ++i) tmp[i] = new_dp1[i];

        /* Add a single part ≡ 3 (mod 5) using the snapshot. */
        for (idx_t s = 3; s <= n; s += 5) {
            for (idx_t k = s; k <= n; ++k) {
                new_dp1[(size_t)k] += tmp[(size_t)(k - s)];
            }
        }
        free(tmp);

        /* ------------------------------------------------------------------
           Compute dp2[n]: partitions with exactly one part ≡ 0 (mod 5) and no
           parts ≡ 2 or 3.
         ------------------------------------------------------------------ */
        for (size_t i = 0; i < needed; ++i) new_dp2[i] = 0;
        for (idx_t s = 5; s <= n; s += 5) {
            for (idx_t k = s; k <= n; ++k) {
                new_dp2[(size_t)k] += new_base[(size_t)(k - s)];
            }
        }

        /* Free old arrays if any. */
        if (base  != NULL) free(base);
        if (dp1   != NULL) free(dp1);
        if (dp2   != NULL) free(dp2);

        /* Store the new arrays and current maximum n. */
        base   = new_base;
        dp1    = new_dp1;
        dp2    = new_dp2;
        max_n  = n;
    }

    /* Return the value for the requested index. */
    return dp1[(size_t)n] + dp2[(size_t)n];
}
