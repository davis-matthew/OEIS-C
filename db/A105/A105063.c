/*
 * A105063  a(1)=0, a(2)=0, a(3)=8, a(4)=24, a(n) = 32 + 66*a(n-2) - a(n-4)
 *          for n > 4.
 * https://oeis.org/A105063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 32 + 66*a(n-2) - a(n-4) for n > 4, with initial values
 *            a(1)=0, a(2)=0, a(3)=8, a(4)=24
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle small indices directly. */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)0;
    if (n == 3) return (seq_t)8;
    if (n == 4) return (seq_t)24;

    /* Sliding window for a(k-4), a(k-3), a(k-2), a(k-1). */
    seq_t a0 = (seq_t)0; /* a(k-4)  */
    seq_t a1 = (seq_t)0; /* a(k-3)  */
    seq_t a2 = (seq_t)8; /* a(k-2)  */
    seq_t a3 = (seq_t)24;/* a(k-1)  */

    for (idx_t k = 5; k <= n; ++k)
    {
        seq_t next = (seq_t)32 + (seq_t)66 * a2 - a0; /* a(k) */
        /* Shift the window forward. */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
