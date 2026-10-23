/*
 * A021873  Decimal expansion of 1/869.
 * https://oeis.org/A021873
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10^(n+1) / 869) mod 10 Computes the nth decimal
 *            digit of 1/869
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* A cache for the repeating cycle of 1/869. */
    static seq_t *period = NULL;
    static idx_t period_len = 0;

    if (period_len == 0) {
        /* Compute the cycle once. */
        const idx_t denom = 869;
        idx_t rem = 1;
        /* Allocate maximum possible period length (denom). */
        period = (seq_t *)malloc(denom * sizeof(seq_t));
        if (!period)
            return 0; /* allocation failed – unlikely for small denom */

        /* Generate digits until the remainder repeats the initial value. */
        do {
            rem *= 10;
            seq_t digit = (seq_t)(rem / denom);  /* digit 0‑9 */
            rem %= denom;
            period[period_len++] = digit;
        } while (rem != 1);
    }

    /* Return the digit at position n using the precomputed cycle. */
    idx_t idx = n % period_len;
    return period[idx];
}
