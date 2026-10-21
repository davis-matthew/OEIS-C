/*
 * A208693  Number of 7 X n 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 1 1 and 1 0 1 vertically.
 * https://oeis.org/A208693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 39*a(n-2) + 9*a(n-3) - 180*a(n-4); initial
 *            values: a(1)=31, a(2)=961, a(3)=2449, a(4)=34627
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Return error value for out of range (not used) */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)31;
    if (n == 2) return (seq_t)961;
    if (n == 3) return (seq_t)2449;
    if (n == 4) return (seq_t)34627;

    seq_t a1 = (seq_t)31;   /* a(n-4) */
    seq_t a2 = (seq_t)961;  /* a(n-3) */
    seq_t a3 = (seq_t)2449; /* a(n-2) */
    seq_t a4 = (seq_t)34627;/* a(n-1) */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t cur = a4 + 39*a3 + 9*a2 - 180*a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = cur;
    }
    return a4;
}
