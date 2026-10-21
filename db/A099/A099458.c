/*
 * A099458  An Alexander sequence for the knot 9_44.
 * https://oeis.org/A099458
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(0)=1, a(1)=4, a(2)=9, a(3)=12, a(4)=-1; for n≥5: a(n) =
 *            4*a(n-1) – 7*a(n-2) + 4*a(n-3) – a(n-4)
 * Range:     i8 n <= 5, i16 n <= 14, i32 n <= 28, i64 n <= 28, i128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* initial values */
    static const seq_t init[5] = { (seq_t)1, (seq_t)4, (seq_t)9,
                                   (seq_t)12, (seq_t)-1 };

    if (n < 5)
        return init[(size_t)n];

    /* a(i-4), a(i-3), a(i-2), a(i-1) for current i */
    seq_t a0 = init[1];  /* a(1)   */
    seq_t a1 = init[2];  /* a(2)   */
    seq_t a2 = init[3];  /* a(3)   */
    seq_t a3 = init[4];  /* a(4)   */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t next = (seq_t)4 * a3  - (seq_t)7 * a2
                   + (seq_t)4 * a1  -      a0;
        /* shift window */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
