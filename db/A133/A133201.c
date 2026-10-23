/*
 * A133201  a(n) = A133195(n)/3.
 * https://oeis.org/A133201
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = a(n-1)+10*a(n-3)-10*a(n-4) initial values: a(0)=0,
 *            a(1)=1, a(2)=2, a(3)=3
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 30, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)3;

    /* Use only four rolling variables – no dynamic allocation */
    seq_t a0 = (seq_t)0;   /* a(n-4)   */
    seq_t a1 = (seq_t)1;   /* a(n-3)   */
    seq_t a2 = (seq_t)2;   /* a(n-2)   */
    seq_t a3 = (seq_t)3;   /* a(n-1)   */
    seq_t ai = a3;         /* current a(n) */

    for (idx_t i = 4; i <= n; ++i) {
        ai = a3 + ((seq_t)10) * a1 - ((seq_t)10) * a0;
        /* shift the window */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = ai;
    }
    return ai;
}
