/*
 * A188865  Number of n X 8 binary arrays without the pattern 0 1 diagonally,
 *          vertically or antidiagonally.
 * https://oeis.org/A188865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) for n>7, with initial values a(1)=256,
 *            a(2)=1393, a(3)=3194, a(4)=5275, a(5)=7442, a(6)=9627,
 *            a(7)=11814, a(8)=14001
 * Range:     u8 none, u16 n <= 31, u32 n <= 200, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    static const seq_t init[8] = {
        (seq_t)256,  (seq_t)1393, (seq_t)3194, (seq_t)5275,
        (seq_t)7442, (seq_t)9627, (seq_t)11814, (seq_t)14001
    };
    if (n <= 8)
        return init[n - 1];

    seq_t prevprev = init[6]; /* a(7) */
    seq_t prev     = init[7]; /* a(8) */
    idx_t i;
    for (i = 9; i <= n; ++i) {
        seq_t next = prev * 2 - prevprev;
        prevprev = prev;
        prev = next;
    }
    return prev;
}
