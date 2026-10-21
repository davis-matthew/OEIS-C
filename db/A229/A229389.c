/*
 * A229389  Number of nX5 0..3 arrays of the sum of the corresponding element,
 *          the element to the east and the element to the south in a larger
 *          (n+1)X6 0..1 array.
 * https://oeis.org/A229389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 101*a(n-1) - 2532*a(n-2) + 10624*a(n-3) - 8192*a(n-4) for
 *            n > 7 Initial values: a(1) = 560 a(2) = 68672 a(3) = 5212236
 *            a(4) = 345788172 a(5) = 22494002188 a(6) = 1451228983308 a(7) =
 *            93250181644300
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for size_t */

seq_t a(idx_t n)
{
    /* handle offset (sequence starts at 1) */
    if (n == (idx_t)0)
        return (seq_t)0;

    /* base values for n = 1..7 */
    static const seq_t init[7] = {
        (seq_t)560,
        (seq_t)68672,
        (seq_t)5212236,
        (seq_t)345788172,
        (seq_t)22494002188,
        (seq_t)1451228983308,
        (seq_t)93250181644300
    };

    if (n <= (idx_t)7)
        return init[n - (idx_t)1];

    /* Use a sliding window of the last four values. */
    seq_t v0 = init[3]; /* a(4)  */
    seq_t v1 = init[4]; /* a(5)  */
    seq_t v2 = init[5]; /* a(6)  */
    seq_t v3 = init[6]; /* a(7)  */

    /* Compute successive terms up to a(n). */
    for (idx_t i = (idx_t)8; i <= n; ++i) {
        seq_t next = (seq_t)101 * v3
                    - (seq_t)2532 * v2
                    + (seq_t)10624 * v1
                    - (seq_t)8192 * v0;
        /* shift the window: a(n-4)->a(n-3), a(n-3)->a(n-2), a(n-2)->a(n-1), a(n-1)->a(n) */
        v0 = v1;
        v1 = v2;
        v2 = v3;
        v3 = next;
    }
    return v3;
}
