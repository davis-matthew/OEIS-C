/*
 * A131700  Sum of all n-digit Cullen numbers.
 * https://oeis.org/A131700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(n) = sum_{m >= 0, digit_len(m*2^m+1) == n} (m*2^m+1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   digit_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_len.h"   /* int digit_len(seq_t x) */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    seq_t sum = (seq_t)0;
    seq_t p   = (seq_t)1;            /* p = 2^m, starts with m = 0 */

    for (idx_t m = 0; ; ++m)
    {
        seq_t cm = ((seq_t)m * p) + (seq_t)1;   /* Cullen number C_m */

        int d = digit_len(cm);

        if (d == (int)n)
            sum += cm;
        else if (d > (int)n)
            break;                            /* further m will only increase digits */

        p <<= 1;                               /* multiply by 2 for next m */
    }

    return sum;
}
