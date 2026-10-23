/*
 * A042735  Denominators of continued fraction convergents to sqrt(897).
 * https://oeis.org/A042735
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 1198*a(n-4) - a(n-8) for n > 7 Initial values: a(0)=1,
 *            a(1)=1, a(2)=19, a(3)=20, a(4)=1179, a(5)=1199, a(6)=22761,
 *            a(7)=23960
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for size_t */

extern seq_t a(idx_t n)
{
    /* Initial eight terms */
    static const seq_t init[8] = {
        (seq_t)1,      /* a(0) */
        (seq_t)1,      /* a(1) */
        (seq_t)19,     /* a(2) */
        (seq_t)20,     /* a(3) */
        (seq_t)1179,   /* a(4) */
        (seq_t)1199,   /* a(5) */
        (seq_t)22761,  /* a(6) */
        (seq_t)23960   /* a(7) */
    };

    /* Return known values directly for n < 8 */
    if (n < (idx_t)8)
        return init[(size_t)n];

    /* Circular buffer holding the last eight computed terms */
    seq_t buf[8];
    for (size_t i = 0; i < 8; ++i)
        buf[i] = init[i];

    /* Compute a(8) .. a(n) using the recurrence */
    for (idx_t i = (idx_t)8; i <= n; ++i)
    {
        size_t idx  = (size_t)i % 8;
        size_t idx4 = (size_t)(i - 4) % 8;
        size_t idx8 = (size_t)(i - 8) % 8;
        buf[idx] = ((seq_t)1198) * buf[idx4] - buf[idx8];
    }

    return buf[(size_t)n % 8];
}
