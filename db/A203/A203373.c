/*
 * A203373  Number of (n+1) X 4 0..1 arrays with every 2 X 2 subblock having
 *          equal diagonal elements or equal antidiagonal elements.
 * https://oeis.org/A203373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 15*a(n-2) + 6*a(n-3) Initial values: a(1)=108,
 *            a(2)=748, a(3)=5208
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first three terms explicitly */
    if (n == 1) return 108;
    if (n == 2) return 748;
    if (n == 3) return 5208;

    /* t1 = a(n-3), t2 = a(n-2), t3 = a(n-1) */
    seq_t t1 = 108;   /* a(1)   */
    seq_t t2 = 748;   /* a(2)   */
    seq_t t3 = 5208;  /* a(3)   */
    seq_t next;

    for (idx_t i = 4; i <= n; ++i)
    {
        next = 9 * t3 - 15 * t2 + 6 * t1;
        t1 = t2;
        t2 = t3;
        t3 = next;
    }

    return t3;
}
