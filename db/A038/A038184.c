/*
 * A038184  State of one-dimensional cellular automaton 'sigma' (Rule 150):
 *          000,001,010,011,100,101,110,111 -> 0,1,1,0,1,0,0,1 at generation
 *          n, converted to a decimal number.
 * https://oeis.org/A038184
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the integer whose binary representation (leftmost bit
 *            first) is the parity of the coefficients of (1 + x + x^2)^n.
 *            Equivalently, a(0)=1 and for n>0 the sequence is obtained by
 *            iterating Rule 150 starting with a single 1 and interpreting the
 *            resulting 0/1 pattern as a binary number
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Maximum number of bits needed for row n: 2*n + 1 */
    size_t max_len = (size_t)(2 * (size_t)n + 1);

    /* Allocate two buffers that will hold consecutive rows. */
    uint8_t *prev = (uint8_t *)malloc(max_len);
    uint8_t *curr = (uint8_t *)malloc(max_len);

    /* Row 0: single 1. */
    prev[0] = 1;
    size_t prev_len = 1;

    /* Generate rows up to the requested n.  Each new row is computed
       by XORing up to three consecutive bits from the previous row. */
    for (idx_t step = 1; step <= n; ++step)
    {
        size_t new_len = 2 * (size_t)step + 1;

        for (size_t j = 0; j < new_len; ++j)
        {
            uint8_t v = 0;
            /* right neighbor (prev[j]) */
            if (j < prev_len)
                v ^= prev[j];
            /* centre neighbor (prev[j-1]) */
            if (j > 0 && j - 1 < prev_len)
                v ^= prev[j - 1];
            /* left neighbour (prev[j-2]) */
            if (j > 1 && j - 2 < prev_len)
                v ^= prev[j - 2];

            curr[j] = v;
        }

        /* Prepare for next iteration: swap buffers. */
        uint8_t *tmp = prev;
        prev = curr;
        curr = tmp;
        prev_len = new_len;
    }

    /* Convert the final binary row to a seq_t value. */
    seq_t result = 0;
    for (size_t i = 0; i < prev_len; ++i)
    {
        result = (result << 1) | (seq_t)prev[i];
    }

    free(prev);
    free(curr);
    return result;
}
