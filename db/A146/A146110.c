/*
 * A146110  Bell numbers (A000110) read mod 20.
 * https://oeis.org/A146110
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Bell numbers modulo 20, computed with the Bell triangle.
   a(0) = 1, a(1) = 1, a(2) = 2, a(3) = 5, …  (see OEIS A146110).
   The sequence is periodic with period 9372.  The values for one period
   are generated once, stored in a static table, and then indexed
   modulo 9372 on subsequent calls. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static const size_t PERIOD = 9372u;
    static int          initialized = 0;
    static seq_t        *table = NULL;

    if (!initialized) {
        /* Allocate storage for one full period of values. */
        table = (seq_t *)malloc(PERIOD * sizeof(seq_t));
        /* Temporary arrays for the Bell triangle rows. */
        unsigned int *prev = (unsigned int *)malloc((PERIOD + 1) * sizeof(unsigned int));
        unsigned int *curr = (unsigned int *)malloc((PERIOD + 1) * sizeof(unsigned int));

        /* First Bell number B(0). */
        const unsigned int MOD = 20u;
        prev[0] = 1 % MOD;
        table[0] = (seq_t)prev[0];

        unsigned int prev_len = 1u;  /* Length of the current row. */

        /* Generate rows 1 … PERIOD-1 of the Bell triangle. */
        for (idx_t row = 1u; row < PERIOD; ++row) {
            /* First element of the new row = last element of previous row. */
            curr[0] = prev[prev_len - 1];

            /* Build the rest of the new row. */
            for (unsigned int k = 1u; k <= prev_len; ++k) {
                unsigned int sum = curr[k - 1] + prev[k - 1];
                if (sum >= MOD) sum -= MOD;   /* reduce modulo 20 */
                curr[k] = sum;
            }

            /* Store the Bell number for this row (first element). */
            table[row] = (seq_t)curr[0];

            /* Prepare for next iteration: swap prev and curr. */
            unsigned int *tmp  = prev;
            prev   = curr;
            curr   = tmp;
            ++prev_len;   /* next row is one element longer */
        }

        /* Clean up temporary arrays; they are no longer needed. */
        free(prev);
        free(curr);

        initialized = 1;
    }

    /* Return the requested value, using the period. */
    return table[(size_t)(n % PERIOD)];
}
