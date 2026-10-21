/*
 * A229529  Number of defective 3-colorings of an n X 3 0..2 array connected
 *          horizontally, diagonally and antidiagonally with exactly one
 *          mistake, and colors introduced in row-major 0..2 order.
 * https://oeis.org/A229529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) - 29*a(n-2) + 20*a(n-3) - 4*a(n-4) (n>5)
 *            Initial values: a(1)=2, a(2)=8, a(3)=58, a(4)=356, a(5)=2038
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Integer cache for the first five terms – initialized lazily. */
static const seq_t INIT[5] = { (seq_t)2, (seq_t)8, (seq_t)58, (seq_t)356, (seq_t)2038 };

seq_t a(idx_t n)
{
    /* sequence is defined for n >= 1; for n == 0 return 0 */
    if (n == 0)
        return (seq_t)0;

    /* Return the initial values directly for n <= 5. */
    if (n <= 5)
        return INIT[n - 1];

    /* Compute a(n) iteratively – O(n) time, O(1) memory. */
    seq_t a_nm4 = INIT[0];   /* a(n-4) */
    seq_t a_nm3 = INIT[1];   /* a(n-3) */
    seq_t a_nm2 = INIT[2];   /* a(n-2) */
    seq_t a_nm1 = INIT[3];   /* a(n-1) */
    seq_t a_i   = INIT[4];   /* a(n) for the current index */

    for (idx_t i = 6; i <= n; ++i) {
        /* a_i = 10*a_{i-1} - 29*a_{i-2} + 20*a_{i-3} - 4*a_{i-4} */
        seq_t next = (seq_t)10 * a_i
                     - (seq_t)29 * a_nm1
                     + (seq_t)20 * a_nm2
                     - (seq_t)4  * a_nm3;

        /* Shift the window for the next iteration. */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_i;
        a_i   = next;
    }

    return a_i;
}
