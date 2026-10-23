/*
 * A026069  a(n) = number of (s(0), s(1), ..., s(n)) such that every s(i) is
 *          an integer, s(0) = 0, |s(i) - s(i-1)| = 1 for i = 1,2; |s(i) -
 *          s(i-1)| <= 1 for i >= 3, s(n) = 2. Also a(n) = T(n,n-2), where T
 *          is the array defined in A024996.
 * https://oeis.org/A026069
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of walks (s(0)…s(n)) with * s(0)=0,
 *            |s(i)-s(i-1)|=1 for i=1,2, * |s(i)-s(i-1)|≤1 for i≥3, and
 *            s(n)=2. * The routine uses dynamic programming over the walk
 *            positions. * Complexity: O(n²) time, O(n) memory
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 23, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* seq_t and idx_t are defined by the build system. */
/* Remember: do NOT add any STORAGE-QUALIFIER to the typedefs below. */
 
seq_t a(idx_t n)
{
    /* The definition is only valid for n ≥ 3; otherwise return 0. */
    if (n < 3)
        return 0;

    /* Positions range from –n to +n (inclusive). */
    size_t size   = (size_t)2 * (size_t)n + 1;   /* array length  */
    size_t offset = (size_t)n;                  /* index of position 0 */

    /* Allocate two DP rows. */
    seq_t *dp   = (seq_t *)calloc(size,   sizeof(seq_t));
    seq_t *next = (seq_t *)calloc(size,   sizeof(seq_t));
    if (!dp || !next) {
        free(dp); free(next);
        return 0;
    }

    /* Start at 0. */
    dp[offset] = 1;

    /* Iterate over each step of the walk. */
    for (idx_t step = 1; step <= n; ++step) {
        /* Reset the next row. */
        memset(next, 0, size * sizeof(seq_t));

        for (size_t idx = 0; idx < size; ++idx) {
            seq_t val = dp[idx];
            if (!val) continue;          /* skip zero entries */

            if (step <= 2) {  /* only ±1 moves */
                if (idx > 0)        next[idx - 1] += val; /* move left   */
                if (idx + 1 < size) next[idx + 1] += val; /* move right  */
            } else {           /* -1, 0, +1 moves */
                if (idx > 0)        next[idx - 1] += val; /* left   */
                next[idx]           += val;               /* stay   */
                if (idx + 1 < size) next[idx + 1] += val; /* right  */
            }
        }

        /* Swap the rows for the next iteration. */
        seq_t *tmp = dp; dp = next; next = tmp;
    }

    /* Answer is the number of ways to be at position 2 after n steps. */
    seq_t result = dp[2 + offset];

    free(dp);
    free(next);
    return result;
}
