/*
 * A079215  Catalan paths: numbers starting with 1 and ending with 1 where
 *          each digit is positive and adjacent digits differ by 1.
 * https://oeis.org/A079215
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 22, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Enumerator of Dyck paths of all lengths sorted lexicographically.
   a(n) is the n‑th decimal number obtained by interpreting the height
   sequence of a Dyck path (starting and ending at 1) as decimal digits. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the semilength k such that
       sum_{i=0}^{k-1} Catalan(i) ≤ n < sum_{i=0}^{k} Catalan(i).  */
    idx_t cum = 0;          /* cumulative count before the current k     */
    idx_t k   = 0;          /* current semilength                         */
    idx_t Ck  = 1;          /* Catalan(k)                                 */

    while (cum + Ck <= n) {
        cum += Ck;
        ++k;
        /* Next Catalan number:
           C_k = C_{k-1} * 2 * (2*(k-1)+1) / (k+1) */
        Ck = Ck * 2 * (2 * (k - 1) + 1) / (k + 1);
    }

    idx_t rank = n - cum;   /* rank within the block of size Ck          */

    /* DP table: dp[steps][h] = number of valid suffixes of length
       'steps' starting from height h (h ≥ 0).  */
    int steps_max = 2 * (int)k;
    int cols = (int)k + 1;
    size_t dp_cells = (size_t)(steps_max + 1) * (size_t)cols;
    idx_t *dp = (idx_t *)calloc(dp_cells, sizeof(idx_t));
    if (!dp) return 0;      /* defensive; OOM unlikely for k ≤ 10       */

    /* Base case: 0 steps remaining – only the empty suffix if h==0. */
    for (int h = 0; h < cols; ++h)
        dp[h] = (h == 0) ? 1 : 0;

    /* Fill DP for steps = 1 … steps_max. */
    for (int steps = 1; steps <= steps_max; ++steps) {
        size_t base = (size_t)steps * (size_t)cols;
        size_t prev = (size_t)(steps - 1) * (size_t)cols;
        for (int h = 0; h < cols; ++h) {
            idx_t val = 0;
            if (h + 1 <= k)                val += dp[prev + (h + 1)];
            if (h > 0)                     val += dp[prev + (h - 1)];
            dp[base + h] = val;
        }
    }

    /* Unrank the path. */
    seq_t result = 1;                 /* first digit is always 1               */
    int h = 0;                        /* current height (digit‑1)             */
    int steps_left = steps_max;

    while (steps_left > 0) {
        idx_t count_D = 0;
        if (h > 0)
            count_D = dp[(size_t)(steps_left - 1) * (size_t)cols + (h - 1)];

        if (rank < count_D) {
            /* choose down step (smaller digit) */
            --h;
        } else {
            /* choose up step */
            rank -= count_D;
            ++h;
        }

        result = result * (seq_t)10 + (seq_t)(h + 1);
        --steps_left;
    }

    free(dp);
    return result;
}
