/*
 * A041093  Denominators of continued fraction convergents to sqrt(54).
 * https://oeis.org/A041093
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 20, u64 n <= 39, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 970*a(n-6)-a(n-12),
   a(0)=1, a(1)=2, a(2)=3, a(3)=20, a(4)=23, a(5)=66,
   a(6)=947, a(7)=1960, a(8)=2907, a(9)=19402,
   a(10)=22309, a(11)=64020 */
#include <stdlib.h>

static const seq_t init[12] = {
    (seq_t)1,  (seq_t)2,  (seq_t)3,  (seq_t)20,
    (seq_t)23, (seq_t)66, (seq_t)947, (seq_t)1960,
    (seq_t)2907, (seq_t)19402, (seq_t)22309, (seq_t)64020
};

seq_t a(idx_t n)
{
    if (n < 12)
        return init[(size_t)n];

    /* sliding window of the last 12 computed terms */
    seq_t buf[12];
    idx_t i;
    for (i = 0; i < 12; ++i)
        buf[i] = init[i];

    for (i = 12; i <= n; ++i)
    {
        /* indices modulo 12 */
        idx_t i6  = (i - 6) % 12;
        idx_t i12 = (i - 12) % 12;

        seq_t val = 970 * buf[i6];
        /* subtraction is safe because 970*a(n-6) > a(n-12) for all n>=12 */
        val -= buf[i12];
        buf[i % 12] = val;
    }

    return buf[n % 12];
}
