/*
 * A021841  Decimal expansion of 1/837.
 * https://oeis.org/A021841
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) is the nth digit after the decimal point in the expansion
 *            of 1/837
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Period of 1/837 is 15 digits: 0 0 1 1 9 4 7 4 3 1 3 0 2 2 7  */
    static seq_t *period = NULL;
    static size_t period_len = 15;

    if (!period) {
        period = (seq_t *)malloc(period_len * sizeof(seq_t));
        if (!period) return 0;  /* fallback if allocation fails */
        idx_t rem = 1;
        for (size_t i = 0; i < period_len; i++) {
            idx_t tmp = rem * 10;        /* tmp < 10*837 = 8370, fits in idx_t */
            seq_t digit = (seq_t)tmp / 837;
            rem = tmp % 837;
            period[i] = digit;
        }
    }

    return period[(size_t)(n % period_len)];
}
