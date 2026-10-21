/*
 * A213209  Number of isolated even numbers in Collatz (3x+1) trajectory of n.
 * https://oeis.org/A213209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of isolated even numbers in Collatz (3x+1)
 *            trajectory of n
 * Range:     u8 n <= 26, u16 n <= 2811, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t count = 0;
    seq_t cur   = (seq_t)n;
    bool prev_is_odd = true;       /* previous symbol is treated as odd for the first element */

    while (1) {
        if (cur == 1)                /* end of trajectory */
            break;

        seq_t next;
        if ((cur & (seq_t)1) == 0) { /* cur is even */
            next = cur >> 1;        /* Collatz successor is half */
            if (prev_is_odd && ((next & (seq_t)1) == 1))
                ++count;           /* isolated: previous odd, next odd */
        } else {                     /* cur is odd */
            next = cur * 3 + 1;
        }

        prev_is_odd = ((cur & (seq_t)1) != 0);
        cur = next;
    }

    return count;
}
