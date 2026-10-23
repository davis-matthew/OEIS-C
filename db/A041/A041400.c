/*
 * A041400  Numerators of continued fraction convergents to sqrt(215).
 * https://oeis.org/A041400
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 19, u64 n <= 39, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(-2) = 0, a(-1) = 1
   For n ≥ 0 set
       t = (n == 0) ? 14
           : (((n-1) % 8 == 3) || ((n-1) % 8 == 7)) ? 28 : 1;
   Then a(n) = t * a(n-1) + a(n-2).
   This yields the numerators of the continued‑fraction convergents to √215. */
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t prevprev = 0;   /* a(-2) */
    seq_t prev = 1;       /* a(-1) */
    seq_t curr = 0;       /* a(n) */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t ak;
        if (k == 0)
            ak = 14;
        else {
            idx_t r = (k - 1) % 8;
            ak = (r == 3 || r == 7) ? 28 : 1;
        }
        curr = ak * prev + prevprev;
        prevprev = prev;
        prev = curr;
    }
    return curr;
}
