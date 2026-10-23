/*
 * A041413  Denominators of continued fraction convergents to sqrt(221).
 * https://oeis.org/A041413
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 3330*a(n-6) - a(n-12); Initial values: a(0)=1, a(1)=1,
 *            a(2)=7, a(3)=15, a(4)=97, a(5)=112, a(6)=3233, a(7)=3345,
 *            a(8)=23303, a(9)=49951, a(10)=323009, a(11)=372960
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for size_t if needed */

seq_t a(idx_t n)
{
    /* Handle the first 12 terms directly. */
    static const seq_t init[12] = {
        (seq_t)1,  (seq_t)1,  (seq_t)7,  (seq_t)15,
        (seq_t)97, (seq_t)112, (seq_t)3233, (seq_t)3345,
        (seq_t)23303, (seq_t)49951, (seq_t)323009, (seq_t)372960
    };

    if (n < 12)
        return init[(size_t)n];

    /* Use a circular buffer of the last 12 values. */
    seq_t buf[12];
    for (size_t i = 0; i < 12; ++i)
        buf[i] = init[i];

    /* Compute terms from 12 up to n inclusively. */
    for (idx_t i = 12; i <= n; ++i)
    {
        /* a(i) = 3330*a(i-6) - a(i-12) */
        seq_t next = (seq_t)3330 * buf[(i - 6) % 12] - buf[(i - 12) % 12];
        buf[i % 12] = next;
    }

    return buf[n % 12];
}
