/*
 * A042338  Numerators of continued fraction convergents to sqrt(696).
 * https://oeis.org/A042338
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 21, u64 n <= 42, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A042338: Numerators of convergents to sqrt(696)
   Recurrence:
     p_{-2} = 0,   p_{-1} = 1
     for k ≥ 0:   p_k = a_k * p_{k-1} + p_{k-2}
   where a_0 = 26 and
         a_k = period[(k-1) mod 8] (for k ≥ 1)
   The sequence term a(n) is p_n.
*/

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Partial quotients of the periodic part of sqrt(696) after the
       integer part 26. */
    static const uint8_t period[8] = { 2, 1, 1, 1, 1, 1, 2, 52 };

    seq_t pp = (seq_t)0;     /* p_{-2} */
    seq_t p  = (seq_t)1;     /* p_{-1} */

    for (idx_t i = 0; i <= n; ++i) {
        seq_t ai = (i == 0) ? (seq_t)26
                            : (seq_t)period[(i - 1) % 8];
        seq_t next = ai * p + pp;  /* p_k = a_k * p_{k-1} + p_{k-2} */
        pp = p;
        p  = next;
    }
    return p;   /* p_n = a(n) */
}
