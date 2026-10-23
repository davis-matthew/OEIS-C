/*
 * A049348  Row sums of triangle A049324.
 * https://oeis.org/A049348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1)=1, a(2)=4, a(3)=10; a(n)=a(n-1)+3*a(n-2)+3*a(n-3) for n≥4
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for n<1 – return 0 as a safe placeholder */

    /* handle the first three values directly */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)10;

    /* iterate up to n, keeping only the last three terms */
    seq_t a1 = (seq_t)1;   /* a(1)   */
    seq_t a2 = (seq_t)4;   /* a(2)   */
    seq_t a3 = (seq_t)10;  /* a(3)   */
    seq_t ai;

    for (idx_t i = 4; i <= n; ++i) {
        ai = a3 + (seq_t)3 * a2 + (seq_t)3 * a1;  /* recurrence */
        a1 = a2;
        a2 = a3;
        a3 = ai;
    }
    return a3;
}
