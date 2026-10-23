/*
 * A033114  Base-4 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,0.
 * https://oeis.org/A033114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = floor(4^(n+1)/15)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=4, a(n) = 3*a(n-1) + 4*a(n-2) + 1  (n≥3) */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(1) and a(2) are the only non‑zero terms with offset 1. */
    if (n <= 0) return (seq_t)0;          /* offset handling – undefined for n<1 */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;

    seq_t a_prev  = (seq_t)1;             /* a(n-2)  */
    seq_t a_curr  = (seq_t)4;             /* a(n-1)  */
    seq_t a_next;

    for (idx_t i = 3; i <= n; ++i) {
        /* a_next = 3*a_curr + 4*a_prev + 1  */
        a_next = a_curr * (seq_t)3 + a_prev * (seq_t)4 + (seq_t)1;
        a_prev = a_curr;
        a_curr = a_next;
    }
    return a_curr;
}
