/*
 * A138289  Row sums of A138060.
 * https://oeis.org/A138289
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-4) Initial values: a(1)=1, a(2)=3, a(3)=6,
 *            a(4)=10 (Row sums of OEIS A138060)
 * Range:     u8 n <= 14, u16 n <= 31, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)6;
    if (n == 4) return (seq_t)10;

    /* Keep the last four values only; no dynamic allocation needed. */
    seq_t a1 = (seq_t)1;  /* a[n-4]  */
    seq_t a2 = (seq_t)3;  /* a[n-3]  */
    seq_t a3 = (seq_t)6;  /* a[n-2]  */
    seq_t a4 = (seq_t)10; /* a[n-1]  */
    seq_t a_new;

    for (idx_t i = 5; i <= n; ++i) {
        a_new = a4 + a1;   /* a[i] = a[i-1] + a[i-4] */

        /* Shift the window for next iteration. */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a_new;
    }

    return a4; /* a[n] is now stored in a4 */
}
