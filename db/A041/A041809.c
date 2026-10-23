/*
 * A041809  Denominators of continued fraction convergents to sqrt(425).
 * https://oeis.org/A041809
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 536*a(n-7) + a(n-14) for n > 13 initial values: a(0) = 1,
 *            a(1) = 1, a(2) = 2, a(3) = 3, a(4) = 5, a(5) = 8, a(6) = 13,
 *            a(7) = 528, a(8) = 541, a(9) = 1069, a(10) = 1610, a(11) = 2679,
 *            a(12) = 4289, a(13) = 6968
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 27, u64 n <= 50, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* Initial 14 terms */
    static const seq_t init[14] = {
        (seq_t)1,   (seq_t)1,   (seq_t)2,   (seq_t)3,   (seq_t)5,
        (seq_t)8,   (seq_t)13,  (seq_t)528, (seq_t)541, (seq_t)1069,
        (seq_t)1610,(seq_t)2679,(seq_t)4289,(seq_t)6968
    };

    /* For indices < 14 just return the pre‑computed value */
    if (n < 14) {
        /* cast n to size_t for array indexing */
        return init[(size_t)n];
    }

    /* Use a circular buffer of length 14 to generate the sequence
       iteratively.  This requires only stack storage and works for
       arbitrary‑precision seq_t. */
    seq_t buf[14];
    size_t i;

    /* Copy the initial terms into the buffer */
    for (i = 0; i < 14; ++i) {
        buf[i] = init[i];
    }

    /* Compute terms from 14 up to n */
    for (i = 14; i <= (size_t)n; ++i) {
        size_t idx7  = (i + 14 - 7) % 14;   /* (i-7) % 14 */
        size_t idx14 = (i + 14 - 14) % 14;  /* (i-14) % 14 */
        /* recurrence: a(i) = 536*a(i-7) + a(i-14) */
        buf[i % 14] = (seq_t)536 * buf[idx7] + buf[idx14];
    }

    /* Result is stored at position n % 14 */
    return buf[(size_t)n % 14];
}
