/*
 * A053250  Coefficients of the '3rd-order' mock theta function phi(q).
 * https://oeis.org/A053250
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, changed
 * Formula:   a(n) = ( number of partitions of n into distinct odd parts with
 *            (largest – 2·parts) ≡ 3 (mod 4) ) minus ( number with (largest –
 *            2·parts) ≡ 1 (mod 4) ), a(0) = 1. This is the coefficient
 *            sequence of the mock‑theta function φ(q)
 * Range:     i8 n <= 148, i16 n <= 470, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes the value for any non‑negative n.  Results are cached so
   repeated calls are fast.  The algorithm enumerates partitions by iterating
   over odd parts in ascending order and keeps separate counts for partitions
   with even and odd numbers of parts.  For each largest part L it adds the
   corresponding contributions to the desired counts.  Complexity: O(N²)
   with N = n, which is below the 2‑second limit for n ≤ 10000.  */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of previously computed values */
    static seq_t *a_cache = NULL;
    static idx_t cache_size = 0;   /* number of computed entries (max index + 1) */

    if (n >= cache_size) {
        /* extend cache to size n+1 */
        size_t new_size = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)calloc(new_size, sizeof(seq_t));
        if (!new_cache) return (seq_t)0;   /* memory failure, return 0 */

        /* compute all values up to n */
        new_cache[0] = 1;   /* a(0) = 1 */

        idx_t N = n;
        size_t size = (size_t)(N + 1);

        /* arrays for dynamic programming */
        seq_t *even = (seq_t *)calloc(size, sizeof(seq_t));
        seq_t *odd  = (seq_t *)calloc(size, sizeof(seq_t));
        if (!even || !odd) {
            free(even); free(odd); free(new_cache);
            return (seq_t)0;
        }
        even[0] = 1;   /* partition of 0 with 0 parts (even) */

        /* tables for counts A027358 and A027357 */
        seq_t *cnt358 = (seq_t *)calloc(size, sizeof(seq_t));
        seq_t *cnt357 = (seq_t *)calloc(size, sizeof(seq_t));
        if (!cnt358 || !cnt357) {
            free(even); free(odd); free(cnt358); free(cnt357);
            free(new_cache);
            return (seq_t)0;
        }

        /* iterate over odd parts: L = 1,3,5,... */
        for (idx_t L = 1; L <= N; L += 2) {
            int r0 = ((int)L - 2) & 3;   /* R when remaining part count is even */
            int r1 = ((int)L - 4) & 3;   /* R when remaining part count is odd  */

            /* add contributions for partitions whose largest part is L */
            for (idx_t m = 0; m <= N - L; ++m) {
                idx_t idx = L + m;      /* target n */
                seq_t ce = even[m];
                seq_t co = odd[m];

                if (r0 == 3) cnt358[idx] += ce;        /* R ≡ 3 (mod 4) */
                else if (r0 == 1) cnt357[idx] += ce;   /* R ≡ 1 (mod 4) */

                if (r1 == 3) cnt358[idx] += co;
                else if (r1 == 1) cnt357[idx] += co;
            }

            /* update DP with the new part L (distinct, odd) */
            for (idx_t j = N; j >= L; --j) {
                seq_t ev = even[j - L];
                seq_t od = odd[j - L];
                even[j] += od;   /* new partitions with even number of parts */
                odd[j]  += ev;   /* new partitions with odd number of parts  */
            }
        }

        /* finalize a(n) = cnt358(n) - cnt357(n) for n ≥ 1 */
        for (size_t i = 1; i < size; ++i) {
            new_cache[i] = cnt358[i] - cnt357[i];
        }

        /* clean up temporary arrays */
        free(even);
        free(odd);
        free(cnt358);
        free(cnt357);

        /* replace old cache */
        free(a_cache);
        a_cache = new_cache;
        cache_size = new_size;
    }

    return a_cache[n];
}
