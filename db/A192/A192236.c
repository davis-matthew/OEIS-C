/*
 * A192236  Coefficient of x in the reduction of the n-th 2nd-kind Chebyshev
 *          polynomial by x^2 -> x+1.
 * https://oeis.org/A192236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 42, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A192236 – coefficient of x in the reduction of the n‑th
   2nd‑kind Chebyshev polynomial by x² → x+1.
   Recurrence:  a(n) = 2*a(n-1) + 2*a(n-2) + 2*a(n-3) – a(n-4)  for n ≥ 5
   Initial values:  a(1)=2, a(2)=4, a(3)=12, a(4)=36
   (equivalently a(n) = 2*A192237(n+2) with A192237 defined by the same recurrence). */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* undefined for n < 1 – return 0 */
    if (n < 1) return (seq_t)0;

    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)12;
    if (n == 4) return (seq_t)36;

    /* Recurrence for n ≥ 5 */
    seq_t a_nm4 = (seq_t)2;   /* a(n-4)  */
    seq_t a_nm3 = (seq_t)4;   /* a(n-3)  */
    seq_t a_nm2 = (seq_t)12;  /* a(n-2)  */
    seq_t a_nm1 = (seq_t)36;  /* a(n-1)  */
    seq_t a_n   = 0;          /* placeholder for a(n) */

    for (idx_t i = 5; i <= n; ++i) {
        a_n = (seq_t)2 * a_nm1
            + (seq_t)2 * a_nm2
            + (seq_t)2 * a_nm3
            - a_nm4;
        /* shift window */
        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
