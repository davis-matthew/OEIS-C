/*
 * A159526  Numerator of Hermite(n, 11/16).
 * https://oeis.org/A159526
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 11*a(n-1) - 128*(n-1)*a(n-2); a(0)=1, a(1)=11
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 7, i64 n <= 13, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *memo = NULL;
static idx_t memo_len = 0;

seq_t a(idx_t n)
{
    /* Allocate (or reallocate) memo table up to n */
    if (memo == NULL || memo_len <= n) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (tmp == NULL) {
            /* Memory allocation failed; abort gracefully */
            abort();
        }
        /* Copy existing values if any */
        if (memo != NULL) {
            memcpy(tmp, memo, memo_len * sizeof(seq_t));
            free(memo);
        }
        /* Initialize base cases */
        if (new_len > 0)     tmp[0] = (seq_t)1;
        if (new_len > 1)     tmp[1] = (seq_t)11;
        /* Compute remaining terms using the recurrence */
        for (idx_t i = 2; i < new_len; ++i) {
            /* factor = 128 * (i-1) */
            seq_t factor = (seq_t)( (seq_t)(i-1) * (seq_t)128ULL );
            tmp[i] = (seq_t)11 * tmp[i-1] - factor * tmp[i-2];
        }
        memo     = tmp;
        memo_len = new_len;
    }
    return memo[n];
}
