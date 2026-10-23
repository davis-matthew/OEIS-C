/*
 * A187067  Let i be in {1,2,3} and let r >= 0 be an integer. Let p = {p_1,
 *          p_2, p_3} = {-2,0,1}, n = 2*r + p_i and define a(-2)=0. Then, a(n)
 *          = a(2*r + p_i) gives the quantity of H_(7,3,0) tiles in a
 *          subdivided H_(7,i,r) tile after linear scaling by the factor x^r,
 *          where x = sqrt(2*cos(Pi/7)).
 * https://oeis.org/A187067
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 40, u32 n <= 78, u64 n <= 153,
 *            u128 n <= 304, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-2) + 2*a(n-4) - a(n-6)  (n ≥ 6)
   Initial values: a(0)=0, a(1)=1, a(2)=1, a(3)=1, a(4)=1, a(5)=2 */

#include <stdlib.h>

/* External definitions are supplied by the build environment:
   - seq_t : unsigned integer type of fixed size or arbitrary‑precision
   - idx_t : unsigned integer type with at least 64 bits          */

seq_t a(idx_t n)
{
    /* Handle the first six terms directly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)1;
    if (n == 4) return (seq_t)1;
    if (n == 5) return (seq_t)2;

    /* For n ≥ 6, compute iteratively up to n. */
    size_t len = (size_t)n + 1;              /* array length needed          */
    seq_t *buf = (seq_t *)malloc(len * sizeof(seq_t));
    if (!buf) return (seq_t)0;               /* out of memory → 0 (rare)   */

    /* Initialize the first six values. */
    buf[0] = (seq_t)0;
    buf[1] = (seq_t)1;
    buf[2] = (seq_t)1;
    buf[3] = (seq_t)1;
    buf[4] = (seq_t)1;
    buf[5] = (seq_t)2;

    /* Compute subsequent terms. */
    for (size_t i = 6; i < len; ++i) {
        buf[i] = buf[i-2] + (seq_t)2 * buf[i-4] - buf[i-6];
    }

    seq_t result = buf[n];
    free(buf);
    return result;
}
