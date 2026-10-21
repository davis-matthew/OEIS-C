/*
 * A230971  Number of (n+2) X (2+2) 0..3 arrays x(i,j) with each element
 *          diagonally or antidiagonally next to at least one element with
 *          value (x(i,j)+1) mod 4, no adjacent elements equal, and upper left
 *          element zero.
 * https://oeis.org/A230971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 8*a(n-2) + 72*a(n-3) + 9*a(n-4) - 81*a(n-5)
 *            for n≥6 Initial values: a(1)=16, a(2)=144, a(3)=1024, a(4)=9216,
 *            a(5)=85264
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset irrelevant for n<1 */
    if (n <= 5) {
        static const seq_t init[5] = {
            (seq_t)16, (seq_t)144, (seq_t)1024,
            (seq_t)9216, (seq_t)85264
        };
        return init[n-1];
    }
    /* iterate recurrence */
    seq_t a1 = (seq_t)16;   /* a(n-5) */
    seq_t a2 = (seq_t)144;  /* a(n-4) */
    seq_t a3 = (seq_t)1024; /* a(n-3) */
    seq_t a4 = (seq_t)9216; /* a(n-2) */
    seq_t a5 = (seq_t)85264;/* a(n-1) */
    for (idx_t i = 6; i <= n; ++i) {
        seq_t plus  = (seq_t)9 * a5 + (seq_t)72 * a3 + (seq_t)9 * a2;
        seq_t minus = (seq_t)8 * a4 + (seq_t)81 * a1;
        seq_t next  = plus - minus;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = next;
    }
    return a5;
}
