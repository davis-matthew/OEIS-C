/*
 * A042558  Numerators of continued fraction convergents to sqrt(808).
 * https://oeis.org/A042558
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the nth convergent of the continued fraction
 *            for sqrt(808) with a0 = 28 and period
 *            [2,2,1,5,1,1,1,1,13,1,1,1,1,5,1,2,2,56]
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 19, u64 n <= 44, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* period after the initial term a0 = 28 */
    static const seq_t period[] = {
        (seq_t) 2, (seq_t) 2, (seq_t) 1, (seq_t) 5, (seq_t) 1, (seq_t) 1,
        (seq_t) 1, (seq_t) 1, (seq_t)13, (seq_t) 1, (seq_t) 1, (seq_t) 1,
        (seq_t) 1, (seq_t) 5, (seq_t) 1, (seq_t) 2, (seq_t) 2, (seq_t)56
    };
    const size_t plen = sizeof(period) / sizeof(period[0]);

    if (n == 0)
        return (seq_t)28;

    seq_t prevprev = 0;
    seq_t prev     = 1;
    seq_t next;

    for (idx_t i = 0; i <= n; ++i) {
        seq_t a_i;
        if (i == 0) {
            a_i = (seq_t)28;
        } else {
            size_t idx = (size_t)((i - 1) % (idx_t)plen);
            a_i = period[idx];
        }
        next = a_i * prev + prevprev;
        prevprev = prev;
        prev = next;
    }
    return prev;
}
