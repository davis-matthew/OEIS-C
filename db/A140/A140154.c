/*
 * A140154  a(1)=1, a(n) = a(n-1) + n^3 if n odd, a(n) = a(n-1) + n^2 if n is
 *          even.
 * https://oeis.org/A140154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 26, u32 n <= 430, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1; a(n) = a(n-1) + n^3 if n is odd,
                                 a(n-1) + n^2 if n is even. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* Undefined for 0, return 0 silently. */

    seq_t total = 0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t ii = (seq_t)i;
        seq_t sq = ii * ii;                /* ii^2 in seq_t */
        if ((i & 1u) == 0)                 /* even */
            total += sq;                   /* add n^2 */
        else
            total += sq * ii;              /* add n^3 */
    }
    return total;
}
