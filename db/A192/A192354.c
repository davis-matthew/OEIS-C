/*
 * A192354  Coefficients of x in the reduction of the polynomial
 *          p(n,x)=(1/2)((x+2)^n+(x-2)^n) by x^2->x+1.
 * https://oeis.org/A192354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 9*a(n-2) - 10*a(n-3) - 5*a(n-4) Initial values
 *            (offset 1): a(1)=0, a(2)=1, a(3)=1, a(4)=14
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)1;
    if (n == 4) return (seq_t)14;

    /* a(n-4), a(n-3), a(n-2), a(n-1) */
    seq_t a1 = (seq_t)0;
    seq_t a2 = (seq_t)1;
    seq_t a3 = (seq_t)1;
    seq_t a4 = (seq_t)14;

    for (idx_t i = 5; i <= n; ++i) {
        /* Use 128‑bit arithmetic to avoid overflow on intermediate products. */
        unsigned __int128 pos = (unsigned __int128)a4 * 2
                              + (unsigned __int128)a3 * 9;
        unsigned __int128 neg = (unsigned __int128)a2 * 10
                              + (unsigned __int128)a1 * 5;
        seq_t next = (seq_t)(pos - neg);   /* pos >= neg for all n≥5 */

        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }

    return a4;
}
