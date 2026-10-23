/*
 * A154021  a(n+2) = 16*a(n+1) - a(n), with a(1)=0, a(2)=4.
 * https://oeis.org/A154021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=0, a(2)=4; a(n)=16*a(n-1)-a(n-2) for n≥3
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A154021 by iterating the recurrence. */
#include <stdlib.h>

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)0;      /* a(1) */
    if (n == 2) return (seq_t)4;      /* a(2) */
    seq_t a_prev = (seq_t)0;          /* a(1) */
    seq_t a_curr = (seq_t)4;          /* a(2) */
    for (idx_t i = 3; i <= n; ++i) {
        seq_t next = (seq_t)16 * a_curr - a_prev;
        a_prev = a_curr;
        a_curr = next;
    }
    return a_curr;
}
