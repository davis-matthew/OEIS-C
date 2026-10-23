/*
 * A035463  Number of partitions of n into parts 8k+4 or 8k+5.
 * https://oeis.org/A035463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of partitions of n into parts 8k+4 or 8k+5
 * Range:     u8 n <= 76, u16 n <= 187, u32 n <= 528, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A simple dynamic programming implementation that recomputes the
   entire table when a larger n is requested.  The table is kept in
   static storage so that repeated calls are O(1) once the maximum
   n has been computed. */

#include <stdlib.h>
#include <stdint.h>

static seq_t *dp = NULL;   /* table: dp[i] = a(i)  (dp[0] = 1)  */
static idx_t max_n = 0;    /* largest n for which dp is valid */

/* -------------------------------------------------------------------- */
/* a(n) - the requested OEIS term                                      */
/* -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* undefined below offset */

    if (n > max_n) {
        /* Allocate (or reallocate) the array for indices 0..n. */
        seq_t *tmp = (seq_t *)realloc(dp, (n + 1) * sizeof(seq_t));
        if (!tmp) abort();                /* allocation failure */

        /* Re‑compute the entire table up to n. */
        for (idx_t i = 0; i <= n; ++i)
            tmp[i] = 0;
        tmp[0] = 1;                       /* empty partition */

        /* Process all allowed parts 8k+4.   */
        for (idx_t part = 4; part <= n; part += 8) {
            for (idx_t i = part; i <= n; ++i)
                tmp[i] += tmp[i - part];
        }

        /* Process all allowed parts 8k+5.   */
        for (idx_t part = 5; part <= n; part += 8) {
            for (idx_t i = part; i <= n; ++i)
                tmp[i] += tmp[i - part];
        }

        dp      = tmp;
        max_n   = n;
    }

    return dp[n];
}
