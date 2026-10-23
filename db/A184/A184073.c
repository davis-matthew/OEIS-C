/*
 * A184073  Number of (n+1) X 2 0..2 arrays with rows and columns in
 *          nondecreasing order and with no 2 X 2 subblock sum differing from
 *          a horizontal or vertical neighbor subblock sum by more than one.
 * https://oeis.org/A184073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1)-36*a(n-2)+84*a(n-3)-126*a(n-4) *
 *            +126*a(n-5)-84*a(n-6)+36*a(n-7)-9*a(n-8)+a(n-9) for n>14 * with
 *            initial terms a(1)=29, a(2)=58, a(3)=150, a(4)=335, a(5)=728, *
 *            a(6)=1499, a(7)=2968, a(8)=5645, a(9)=10331, a(10)=18242, *
 *            a(11)=31173, a(12)=51709, a(13)=83492, a(14)=131554
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 59, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n)
{
    /* Handle the first 14 terms explicitly. */
    static const seq_t init[14] = {
        (seq_t)29,    (seq_t)58,   (seq_t)150,  (seq_t)335,
        (seq_t)728,   (seq_t)1499, (seq_t)2968, (seq_t)5645,
        (seq_t)10331, (seq_t)18242,(seq_t)31173, (seq_t)51709,
        (seq_t)83492, (seq_t)131554
    };
    if (n <= 14)
        return init[n-1];

    /* last[0] = a(cur-9), ..., last[8] = a(cur-1) */
    seq_t last[9];
    last[0] = init[5];  /* a(6) */
    last[1] = init[6];  /* a(7) */
    last[2] = init[7];  /* a(8) */
    last[3] = init[8];  /* a(9) */
    last[4] = init[9];  /* a(10) */
    last[5] = init[10]; /* a(11) */
    last[6] = init[11]; /* a(12) */
    last[7] = init[12]; /* a(13) */
    last[8] = init[13]; /* a(14) */

    seq_t next;
    for (idx_t cur = 15; cur <= n; ++cur) {
        next =  (9   * last[8])
               - (36  * last[7])
               + (84  * last[6])
               - (126 * last[5])
               + (126 * last[4])
               - (84  * last[3])
               + (36  * last[2])
               - (9   * last[1])
               + last[0];
        /* shift window */
        last[0] = last[1];
        last[1] = last[2];
        last[2] = last[3];
        last[3] = last[4];
        last[4] = last[5];
        last[5] = last[6];
        last[6] = last[7];
        last[7] = last[8];
        last[8] = next;
    }
    return next;
}
