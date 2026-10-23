/*
 * A135784  a(n) = A000404(n)^2.
 * https://oeis.org/A135784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000404(n)^2 (A000404 = the sequence of positive integers
 *            that can be represented as a sum of two positive squares)
 * Range:     u8 n <= 5, u16 n <= 85, u32 n <= 1400, u64 n <= 1400,
 *            u128 n <= 1400, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Static storage for the list of representable numbers
       (A000404 values).  The list is regenerated if it is too short
       for the requested n. */
    static seq_t *inner = NULL;          /* array of A000404 values   */
    static size_t inner_count = 0;       /* number of values stored  */

    /* Ensure that we have at least 'n' values in the table. */
    if (inner_count < n) {
        /* Free previous allocation if any. */
        free(inner);
        inner = NULL;
        inner_count = 0;

        /* Choose an initial limit for generating the sums. */
        size_t limit = n * 10;                /* crude estimate */
        if (limit < 1000) limit = 1000;       /* ensure some minimum */

        /* We will grow the limit until we have enough numbers. */
        while (1) {
            /* Allocate a bit‑array of size limit+1. */
            bool *mark = (bool*)calloc(limit + 1, sizeof(bool));
            if (!mark) return 0;               /* allocation failure */

            size_t sqrt_limit = (size_t)sqrt((double)limit);

            /* Mark all sums a^2 + b^2 with a >= 1, b >= a, both positive. */
            for (size_t a = 1; a <= sqrt_limit; ++a) {
                size_t a2 = a * a;
                size_t max_b = (size_t)sqrt((double)(limit - a2));
                for (size_t b = a; b <= max_b; ++b) {
                    size_t sum = a2 + b * b;
                    if (sum <= limit) mark[sum] = true;
                }
            }

            /* Count how many representable numbers we found. */
            size_t cnt = 0;
            for (size_t i = 2; i <= limit; ++i)
                if (mark[i]) ++cnt;

            /* If we have enough, extract them into the table. */
            if (cnt >= (size_t)n) {
                /* Allocate the table. */
                inner = (seq_t*)malloc((cnt + 1) * sizeof(seq_t));
                if (!inner) { free(mark); return 0; }

                size_t pos = 0;
                for (size_t i = 2; i <= limit; ++i)
                    if (mark[i]) inner[pos++] = (seq_t)i;
                inner_count = pos;   /* store the actual count */

                free(mark);
                break;              /* table ready */
            }

            /* Not enough: increase the limit and try again. */
            free(mark);
            limit *= 2;
        }
    }

    /* Retrieve the n‑th representable number and square it. */
    seq_t val = inner[n - 1];   /* table is 0‑based; n is 1‑based */
    return val * val;
}
