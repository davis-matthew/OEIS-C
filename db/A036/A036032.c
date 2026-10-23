/*
 * A036032  Number of partitions of n into parts not of form 4k+2, 24k, 24k+7
 *          or 24k-7. Also number of partitions in which no odd part is
 *          repeated, with at most 3 parts of size less than or equal to 2 and
 *          where differences between parts at distance 5 are greater than 1
 *          when the smallest part is odd and great...
 * https://oeis.org/A036032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts not of the * form
 *            4k+2, 24k, 24k+7 or 24k-7. The implementation uses the * classic
 *            unbounded‑coin DP over the allowed residue classes modulo 24. *
 *            Allowed residue classes: 1,3,4,5,8,9,11,12,13,15,16,19,20,21,23
 * Range:     u8 n <= 23, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Allowed part sizes modulo 24. */
static const idx_t residues[15] = {
    1, 3, 4, 5, 8, 9, 11, 12, 13, 15, 16, 19, 20, 21, 23
};

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; return 0 for n == 0. */
    if (n == 0)
        return (seq_t)0;

    /* Allocate an array of n+1 cells, initialized to 0. */
    size_t N = (size_t)n + 1;
    seq_t *dp = (seq_t *)calloc(N, sizeof(seq_t));
    if (!dp)            /* allocation failed – oops but not expected. */
        return (seq_t)0;

    dp[0] = (seq_t)1;   /* one way to partition 0 */

    /* Process each allowed part size. */
    for (int r = 0; r < 15; ++r) {
        for (idx_t s = residues[r]; s <= n; s += 24) {
            for (idx_t i = s; i <= n; ++i) {
                dp[(size_t)i] += dp[(size_t)(i - s)];
            }
        }
    }

    seq_t result = dp[(size_t)n];
    free(dp);
    return result;
}
