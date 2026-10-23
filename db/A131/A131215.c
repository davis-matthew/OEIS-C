/*
 * A131215  Numbers which are both 11-gonal and centered 11-gonal.
 * https://oeis.org/A131215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers which are both 11-gonal and centered 11-gonal (OEIS A131215)
   Recurrence: a(n+3) = 399*a(n+2) - 399*a(n+1) + a(n),
   initial values: a(1)=1, a(2)=606, a(3)=241396
*/
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the first three terms directly */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)606;
    if (n == 3) return (seq_t)241396;

    seq_t a1 = (seq_t)1;          /* a(n-3)   */
    seq_t a2 = (seq_t)606;        /* a(n-2)   */
    seq_t a3 = (seq_t)241396;     /* a(n-1)   */
    seq_t next;

    /* Iterate from term 4 up to the requested n,
       applying the linear recurrence of order 3 */
    for (idx_t i = 4; i <= n; ++i) {
        next = 399 * a3 - 399 * a2 + a1;  /* a(n) = 399*a(n-1) - 399*a(n-2) + a(n-3) */
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
