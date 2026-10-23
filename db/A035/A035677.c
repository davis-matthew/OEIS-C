/*
 * A035677  Number of partitions of n into parts 8k and 8k + 6 with at least
 *          one part of each type.
 * https://oeis.org/A035677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p_all(n) – p_8(n) – p_6(n) where p_all(n) = number of
 *            partitions of n into parts 8k and 8k+6, p_8(n) = number of
 *            partitions of n into multiples of 8 only, p_6(n) = number of
 *            partitions of n into parts 8k+6 only, and the value counts
 *            partitions that contain at least one part of each type
 * Range:     u8 n <= 89, u16 n <= 205, u32 n <= 569, u64 n <= 1773,
 *            u128 n <= 6047, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *dp_all = NULL;
    static seq_t *dp8   = NULL;
    static seq_t *dp6   = NULL;
    static idx_t current_max = 0;

    if (n == 0) return 0;                    /* safety for n < 1 */

    /* Re‑compute data if a larger n is requested */
    if (n > current_max) {
        /* free old arrays */
        if (dp_all) free(dp_all);
        if (dp8)   free(dp8);
        if (dp6)   free(dp6);

        /* allocate new arrays */
        size_t sz = (size_t)(n + 1);
        dp_all = (seq_t*)calloc(sz, sizeof(seq_t));
        dp8   = (seq_t*)calloc(sz, sizeof(seq_t));
        dp6   = (seq_t*)calloc(sz, sizeof(seq_t));

        if (!dp_all || !dp8 || !dp6) return 0;  /* allocation failed */

        /* base case: empty partition */
        dp_all[0] = 1;
        dp8[0]    = 1;
        dp6[0]    = 1;

        /* partitions using only parts 8k+6 (i.e., 6,14,22,…) */
        for (idx_t s = 6; s <= n; s += 8) {
            for (idx_t j = s; j <= n; ++j) {
                dp6[j] += dp6[j - s];
            }
        }

        /* partitions using only multiples of 8 (i.e., 8,16,24,…) */
        for (idx_t s = 8; s <= n; s += 8) {
            for (idx_t j = s; j <= n; ++j) {
                dp8[j] += dp8[j - s];
            }
        }

        /* partitions using both kinds of parts */
        for (idx_t s = 6; s <= n; s += 8) {
            for (idx_t j = s; j <= n; ++j) {
                dp_all[j] += dp_all[j - s];
            }
        }
        for (idx_t s = 8; s <= n; s += 8) {
            for (idx_t j = s; j <= n; ++j) {
                dp_all[j] += dp_all[j - s];
            }
        }

        current_max = n;
    }

    /* result: partitions with at least one part of each type */
    seq_t result = dp_all[n] - dp8[n] - dp6[n];
    return result;
}
