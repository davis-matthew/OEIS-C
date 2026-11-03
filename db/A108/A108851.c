/*
 * A108851  a(n) = 4*a(n-1) + 3*a(n-2), a(0) = 1, a(1) = 2.
 * https://oeis.org/A108851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 58,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 763 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 4*a(n-1) + 3*a(n-2), a(0) = 1, a(1) = 2 */
#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    /* Memoisation */
    static seq_t *dp = NULL;      /* array of computed values */
    static idx_t size = 0;        /* current length of dp */

    /* initialise on first call */
    if (dp == NULL) {
        size = 2;
        dp = mem_new(seq_t, size);
        dp[0] = (seq_t)1;
        dp[1] = (seq_t)2;
    }

    /* extend the memoisation table if necessary */
    if ((idx_t)n >= size) {
        idx_t new_size = n + 1;
        seq_t *tmp = mem_resize(seq_t, dp, new_size);
        if (tmp == NULL) return 0;   /* allocation failure – unlikely */
        dp = tmp;
        for (idx_t i = size; i <= n; ++i) {
            dp[i] = (seq_t)4 * dp[i - 1] + (seq_t)3 * dp[i - 2];
        }
        size = new_size;
    }

    return dp[n];
}
