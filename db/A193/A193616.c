/*
 * A193616  Integers often used as card values in Planning Poker decks.
 * https://oeis.org/A193616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=0,a(2)=1,a(3)=2,a(4)=3,a(5)=5,a(6)=8,a(7)=13,a(8)=20;
   For n≥9: a(n)=2·a(n−1) if n is odd,
   otherwise a(n)=5·a(n−1)/2. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static const seq_t base[8] = {0, 1, 2, 3, 5, 8, 13, 20};

    if (n <= 8)
        return base[n - 1];

    seq_t val = 20;               /* a(8) */
    for (idx_t k = 9; k <= n; ++k) {
        if (k & 1)                /* odd k → multiply by 2 */
            val = val * 2;
        else                      /* even k → multiply by 5 then divide by 2 */
            val = val * 5 / 2;
    }
    return val;
}
