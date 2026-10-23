/*
 * A042499  Denominators of continued fraction convergents to sqrt(777).
 * https://oeis.org/A042499
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 446*a(n-4) - a(n-8)
   with initial values:
   a(0)=1, a(1)=1, a(2)=7, a(3)=8,
   a(4)=439, a(5)=447, a(6)=3121, a(7)=3568 */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* initial 8 terms */
    static const seq_t init[8] = {
        (seq_t)1, (seq_t)1, (seq_t)7, (seq_t)8,
        (seq_t)439, (seq_t)447, (seq_t)3121, (seq_t)3568
    };

    if (n < 8)
        return init[(size_t)n];

    seq_t buf[8];
    for (size_t i = 0; i < 8; ++i)
        buf[i] = init[i];

    for (idx_t i = 8; i <= n; ++i) {
        size_t i4 = (size_t)(i - 4) & 7;
        size_t i8 = (size_t)(i - 8) & 7;
        seq_t next = (seq_t)446 * buf[i4] - buf[i8];
        buf[(size_t)i & 7] = next;
    }
    return buf[(size_t)n & 7];
}
