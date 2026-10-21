/*
 * A085640  Resultant of the polynomial x^3-1 and the Chebyshev polynomial of
 *          the first kind T_n(x).
 * https://oeis.org/A085640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(1) = 1, a(2) = 7, a(3) = 37, a(4) = 193,
   a(n) = 4*a(n‑1) + 6*a(n‑2) + 4*a(n‑3) – a(n‑4)      for n ≥ 5 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /*  small index handling */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)7;
    if (n == 3) return (seq_t)37;
    if (n == 4) return (seq_t)193;

    /* last four values of the sequence */
    seq_t a_nm4 = (seq_t)1;   /* a(n-4) */
    seq_t a_nm3 = (seq_t)7;   /* a(n-3) */
    seq_t a_nm2 = (seq_t)37;  /* a(n-2) */
    seq_t a_nm1 = (seq_t)193; /* a(n-1) */
    seq_t a_n;                /* a(n) */

    for (idx_t i = 5; i <= n; ++i)
    {
        /* recurrence */
        a_n = (seq_t)4 * a_nm1 + (seq_t)6 * a_nm2 + (seq_t)4 * a_nm3 - a_nm4;

        /* shift the window for the next step */
        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_n;
}
