/*
 * A033115  Base-5 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,0.
 * https://oeis.org/A033115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 5*a(n-1)+a(n-2)-5*a(n-3) (a(1)=1,a(2)=5,a(3)=26)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* safety guard – the OEIS offset is 1, but callers may pass 0 */
    if (n == 0) {
        return (seq_t)0;
    }

    /* handle the first three terms explicitly */
    if (n == 1) {
        return (seq_t)1;
    }
    if (n == 2) {
        return (seq_t)5;
    }
    if (n == 3) {
        return (seq_t)26;
    }

    /* iterative computation using only three variables */
    seq_t prev3 = (seq_t)1;   /* a(1) */
    seq_t prev2 = (seq_t)5;   /* a(2) */
    seq_t prev1 = (seq_t)26;  /* a(3) */
    seq_t curr   = (seq_t)0;  /* placeholder */

    /* compute a(4) through a(n) */
    for (idx_t i = 4; i <= n; ++i) {
        curr = prev1 * (seq_t)5 + prev2 - prev3 * (seq_t)5;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = curr;
    }

    return prev1;   /* this is a(n) after the loop */
}
