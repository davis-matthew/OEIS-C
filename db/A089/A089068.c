/*
 * A089068  a(n) = a(n-1)+a(n-2)+a(n-3)+2 with a(0)=0, a(1)=0 and a(2)=1.
 * https://oeis.org/A089068
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3) + 2 with a(0) = 0, a(1) = 0,
 *            a(2) = 1
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first three terms explicitly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;

    /* Iteratively compute subsequent terms. */
    seq_t t0 = (seq_t)0;   /* a(n-3) */
    seq_t t1 = (seq_t)0;   /* a(n-2) */
    seq_t t2 = (seq_t)1;   /* a(n-1) */
    seq_t next;

    for (idx_t i = 3; i <= n; ++i) {
        next = t0 + t1 + t2 + ((seq_t)2);
        t0 = t1;
        t1 = t2;
        t2 = next;
    }
    return t2;
}
