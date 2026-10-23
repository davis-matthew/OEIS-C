/*
 * A008899  x -> x/2 if x even, x -> 3x - 1 if x odd.
 * https://oeis.org/A008899
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 87;  a(n) = a(n-1)/2 if a(n-1) even, else 3*a(n-1)-1   */
#include <stddef.h>

/* Sequence value type, supplied by the build system. */
extern  seq_t a(idx_t n)
{
    /* Start value a(0) */
    if (n == 0)
        return (seq_t)87;

    idx_t i;
    seq_t v = (seq_t)87;          /* current value, initially a(0) */

    for (i = 1; i <= n; ++i) {
        if ((v & (seq_t)1) == (seq_t)0) {   /* even */
            v = v / (seq_t)2;
        } else {                            /* odd */
            v = v * (seq_t)3 - (seq_t)1;
        }
    }
    return v;
}
