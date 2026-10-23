/*
 * A136401  a(n) = 3*a(n-1) - 4*a(n-2) + 6*a(n-3) - 4*a(n-4), with initial
 *          terms 0,0,0,1.
 * https://oeis.org/A136401
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 4*a(n-2) + 6*a(n-3) - 4*a(n-4), initial
 *            values: a(0)=0, a(1)=0, a(2)=0, a(3)=1
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Handle the first four terms directly. */
    if (n == 0 || n == 1 || n == 2)
        return (seq_t)0;
    if (n == 3)
        return (seq_t)1;

    /* Maintain the last four values. */
    seq_t p0 = (seq_t)0;            /* a(i-4) */
    seq_t p1 = (seq_t)0;            /* a(i-3) */
    seq_t p2 = (seq_t)0;            /* a(i-2) */
    seq_t p3 = (seq_t)1;            /* a(i-1) */

    for (idx_t i = 4; i <= n; ++i) {
        /* Compute a(i) using the recurrence. */
        seq_t next = p3 * ((seq_t)3)
                   - p2 * ((seq_t)4)
                   + p1 * ((seq_t)6)
                   - p0 * ((seq_t)4);

        /* Slide the window of last four values. */
        p0 = p1;
        p1 = p2;
        p2 = p3;
        p3 = next;
    }
    return p3;
}
