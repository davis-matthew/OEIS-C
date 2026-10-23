/*
 * A195020  Vertex number of a square spiral in which the length of the first
 *          two edges are the legs of the primitive Pythagorean triple [3, 4,
 *          5]. The edges of the spiral have length A195019.
 * https://oeis.org/A195020
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n*(7*n+13) + (2*n-5)*(-1)^n + 5)/16
 * Range:     u8 n <= 16, u16 n <= 272, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use a signed 128‑bit integer for safe intermediate arithmetic. */
    __int128 tn = ( __int128 ) n;
    __int128 t  = 14 * tn * tn + 26 * tn;          /* 2*n*(7*n+13) */

    /* Add or subtract (2*n-5) according to the parity of n. */
    if ((n & 1) == 0)         /* n even: (-1)^n = +1  */
        t += (2 * tn - 5);
    else                      /* n odd : (-1)^n = -1  */
        t -= (2 * tn - 5);

    t += 5;                    /* add the constant term */

    /* The result is guaranteed to be a non‑negative integer. */
    t /= 16;

    return (seq_t) t;
}
