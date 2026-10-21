/*
 * A200044  Starting with a(1)=1, a(n) = n*a(n-1) if n>= a(n-1), otherwise
 *          a(n) = a(n-1)-n.
 * https://oeis.org/A200044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 416, u32 n <= 9119, u64 n <= 9119,
 *            u128 n <= 9119, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1; for n>=2: if n >= a(n-1) then a(n)=n*a(n-1) else a(n)=a(n-1)-n */
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                /* undefined for n<1, return 0 as a guard */

    seq_t val = (seq_t)1;              /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t si = (seq_t)i;
        if (si >= val) {
            val = si * val;
        } else {
            val = val - si;
        }
    }
    return val;
}
