/*
 * A085059  a(1) = 1, a(n+1) = a(n)-n if a(n) > n else a(n+1) = a(n) + n.
 * https://oeis.org/A085059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   not stated
 * Range:     u8 n <= 146, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1;
   a(n+1) = a(n) - n  if a(n) > n
           a(n) + n  otherwise
   Offset: 1 (a(1) is the first term) */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The function works for any seq_t (N‑bit unsigned or arbitrary‑precision)
       and for any idx_t (unsigned 64‑bit).  No dynamic allocation is required. */
    if (n == 0u)
        return (seq_t)0;        /* Value of a(0) is unused. */

    seq_t val = (seq_t)1;      /* a(1) = 1 */
    for (idx_t i = 1; i < n; ++i) {
        seq_t step = (seq_t)i;
        val = (val > step) ? (val - step) : (val + step);
    }
    return val;
}
