/*
 * A042375  Denominators of continued fraction convergents to sqrt(714).
 * https://oeis.org/A042375
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 14, u32 n <= 27, u64 n <= 49, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A042375 – Denominators of continued fraction convergents to sqrt(714)
    Recurrence: a(n) = 8230*a(n-10) - a(n-20),  for n >= 20
    Initial values (offset 0):
    a(0)=1, a(1)=1, a(2)=3, a(3)=4, a(4)=7, a(5)=18, a(6)=25, a(7)=43,
    a(8)=111, a(9)=154, a(10)=8119, a(11)=8273, a(12)=24665, a(13)=32938,
    a(14)=57603, a(15)=148144, a(16)=205747, a(17)=353891, a(18)=913529,
    a(19)=1267420
    The function returns the exact term a(n) for all n >= 0 in any order
    of calls.  It works for any seq_t and idx_t supplied by the build.    */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /*  Initial 20 terms of the sequence.  */
    static const seq_t init[20] = {
        1,   1,   3,   4,   7,
        18,  25,  43,  111, 154,
        8119,8273,24665,32938,57603,
        148144,205747,353891,913529,1267420
    };

    /*  For indices below 20 we can answer directly.  */
    if (n < 20)
        return init[n];

    /*  Compute terms up to n using the recurrence.
        Allocate an array large enough for indices 0..n.  */
    size_t sz = (size_t)(n + 1);
    seq_t *buf = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!buf)
        return (seq_t)0;   /*  Allocation failed – return zero.  */

    /*  Populate the first 20 terms.  */
    for (size_t i = 0; i < 20; ++i)
        buf[i] = init[i];

    /*  Extend the sequence using the given linear recurrence.  */
    for (size_t i = 20; i <= n; ++i)
        buf[i] = (seq_t)8230 * buf[i - 10] - buf[i - 20];

    seq_t result = buf[n];
    free(buf);
    return result;
}
