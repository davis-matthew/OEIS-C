/*
 * A218987  Power ceiling sequence of 2+sqrt(7).
 * https://oeis.org/A218987
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A218987: Power ceiling sequence of 2+sqrt(7).
   Recurrence:  a(n) = 5*a(n-1) - a(n-2) - 3*a(n-3)
   Initial values: a(0)=5, a(1)=24, a(2)=112. */
#include <stddef.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)5;
    if (n == 1) return (seq_t)24;
    if (n == 2) return (seq_t)112;

    seq_t a0 = (seq_t)5;    /* a(n-3)   */
    seq_t a1 = (seq_t)24;   /* a(n-2)   */
    seq_t a2 = (seq_t)112;  /* a(n-1)   */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t next = 5 * a2 - a1 - 3 * a0;
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
