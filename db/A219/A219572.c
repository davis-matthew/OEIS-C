/*
 * A219572  Number of n X 2 arrays of the minimum value of corresponding
 *          elements and their horizontal or vertical neighbors in a random,
 *          but sorted with lexicographically nondecreasing rows and
 *          nonincreasing columns, 0..3 n X 2 array.
 * https://oeis.org/A219572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 54, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A219572: 
   a(n) = 9*a(n-1) - 36*a(n-2) + 84*a(n-3) - 126*a(n-4)
          + 126*a(n-5) - 84*a(n-6) + 36*a(n-7) - 9*a(n-8) + a(n-9)
   for n > 10.
   Initial values: a(1)=4, a(2)=13, a(3)=59, a(4)=181, a(5)=469,
                   a(6)=1107, a(7)=2447, a(8)=5120, a(9)=10197, a(10)=19415.
*/

#include <stddef.h>

static const seq_t init_vals[10] = {
    (seq_t)4,   (seq_t)13,   (seq_t)59,   (seq_t)181,
    (seq_t)469, (seq_t)1107, (seq_t)2447, (seq_t)5120,
    (seq_t)10197, (seq_t)19415
};

seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0;                /* undefined for n<1 */

    if (n <= (idx_t)10)                /* return stored initial terms */
        return init_vals[n - (idx_t)1];

    /* a(n) for n>10 is computed iteratively using the recurrence.
       We keep the last nine values in the circular buffer `buf`. */
    seq_t buf[9];
    /* fill buf[0] = a(n-1), buf[1] = a(n-2), ..., buf[8] = a(n-9) */
    buf[0] = init_vals[9];   /* a(10) */
    buf[1] = init_vals[8];   /* a(9)  */
    buf[2] = init_vals[7];   /* a(8)  */
    buf[3] = init_vals[6];   /* a(7)  */
    buf[4] = init_vals[5];   /* a(6)  */
    buf[5] = init_vals[4];   /* a(5)  */
    buf[6] = init_vals[3];   /* a(4)  */
    buf[7] = init_vals[2];   /* a(3)  */
    buf[8] = init_vals[1];   /* a(2)  */

    for (idx_t cur = (idx_t)11; cur <= n; ++cur) {
        seq_t v = (seq_t)9  * buf[0]
                - (seq_t)36 * buf[1]
                + (seq_t)84 * buf[2]
                - (seq_t)126 * buf[3]
                + (seq_t)126 * buf[4]
                - (seq_t)84 * buf[5]
                + (seq_t)36 * buf[6]
                - (seq_t)9  * buf[7]
                +         buf[8];

        /* shift buffer right and insert new value at position 0 */
        for (int k = 8; k > 0; --k)
            buf[k] = buf[k - 1];
        buf[0] = v;
    }
    return buf[0];          /* this is a(n) after the loop */
}
