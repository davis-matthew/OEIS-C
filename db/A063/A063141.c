/*
 * A063141  Dimension of the space of weight 2n cusp forms for Gamma_0( 73 ).
 * https://oeis.org/A063141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2) + a(n-3) - a(n-5) Initial values: a(1)=5, a(2)=18,
 *            a(3)=30, a(4)=42, a(5)=54, a(6)=68
 * Range:     u8 n <= 21, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Return 0 for non‑positive indices (undefined behaviour in OEIS) */
    if (n < 1)
        return (seq_t)0;

    /* Handle the first six terms directly */
    if (n <= 6) {
        static const seq_t init[7] = {0, 5, 18, 30, 42, 54, 68};
        return init[n];
    }

    /* Initialise the sliding window with the values a(2) … a(6)  */
    seq_t v0 = 18;   /* a(n-5)   for n = 7   */
    seq_t v1 = 30;   /* a(n-4)   for n = 7   */
    seq_t v2 = 42;   /* a(n-3)   for n = 7   */
    seq_t v3 = 54;   /* a(n-2)   for n = 7   */
    seq_t v4 = 68;   /* a(n-1)   for n = 7   */

    seq_t next;
    for (idx_t i = 7; i <= n; ++i) {
        /* a(i) = a(i-2) + a(i-3) - a(i-5)  -> v3 + v2 - v0   */
        next = v3 + v2 - v0;

        /* shift window for the next iteration                */
        v0 = v1;
        v1 = v2;
        v2 = v3;
        v3 = v4;
        v4 = next;
    }

    return next;
}
