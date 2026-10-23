/*
 * A042656  Numerators of continued fraction convergents to sqrt(858).
 * https://oeis.org/A042656
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A042656 – Numerators of continued‑fraction convergents to sqrt(858)
   Recurrence: a(n) = 1406*a(n-4) – a(n-8)
   Initial values: a(0)=29, a(1)=88, a(2)=205, a(3)=703, a(4)=40979,
                   a(5)=123640, a(6)=288259, a(7)=988417                     */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* Initial values for indices 0 … 7 */
    static const seq_t init[8] = {
        (seq_t)  29,
        (seq_t)  88,
        (seq_t) 205,
        (seq_t) 703,
        (seq_t)40979,
        (seq_t)123640,
        (seq_t)288259,
        (seq_t)988417
    };

    if (n < 8)                /* direct lookup for the first eight terms    */
        return init[(size_t)n];

    /* Circular buffer of the last 8 terms.  After initialization it
       contains a(0) … a(7).                                                       */
    seq_t buf[8];
    for (size_t i = 0; i < 8; ++i)
        buf[i] = init[i];

    /* Compute successively a(8), a(9), … , a(n).  The buffer will always
       hold the last eight computed values: positions correspond to their
       indices modulo 8.                                                         */
    for (idx_t i = 8; i <= n; ++i)
    {
        /* indices of the needed previous terms */
        size_t pos4 = (size_t)((i - 4) & 7u);  /* a(i-4) */
        size_t pos8 = (size_t)((i - 8) & 7u);  /* a(i-8) */

        /* recurrence: a(i) = 1406*a(i-4) – a(i-8) */
        seq_t next = buf[pos4] * (seq_t)1406 - buf[pos8];

        /* store the new value, replacing the oldest of the window (a(i-8)) */
        buf[(size_t)(i & 7u)] = next;
    }

    /* The desired term a(n) resides at position n mod 8 in the buffer.      */
    return buf[(size_t)(n & 7u)];
}
