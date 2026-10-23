/*
 * A041426  Numerators of continued fraction convergents to sqrt(229).
 * https://oeis.org/A041426
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 13, u64 n <= 25, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A041426: numerators of the continued fraction convergents to sqrt(229). */
/* Recurrence:
   p_{-2} = 0,   p_{-1} = 1,
   p_k   = a_k * p_{k-1} + p_{k-2},
   where a_0 = 15 and for k > 0 the partial quotients repeat
   the period 7, 1, 1, 7, 30.  */
#include <stdint.h>

seq_t a(idx_t n) {
    seq_t prevprev = 0;   // p_{k-2}
    seq_t prev     = 1;   // p_{k-1}
    seq_t curr     = 0;   // p_k
    const uint8_t period[5] = {7, 1, 1, 7, 30};

    for (idx_t k = 0; k <= n; ++k) {
        seq_t aik;
        if (k == 0) {
            aik = 15;          // a_0
        } else {
            aik = period[(k - 1) % 5];   // periodic a_k for k >= 1
        }
        curr = aik * prev + prevprev;
        prevprev = prev;
        prev = curr;
    }
    return curr;
}
