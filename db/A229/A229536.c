/*
 * A229536  Number of defective 3-colorings of a 3 X n 0..2 array connected
 *          horizontally, diagonally and antidiagonally with exactly one
 *          mistake, and colors introduced in row-major 0..2 order.
 * https://oeis.org/A229536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence for A229536:
   a(1)=0, a(2)=20, a(3)=58, a(4)=361, a(5)=1588, a(6)=7460,
   a(7)=33136, a(8)=146300.
   For n>8:
   a(n) = 6*a(n-1) - a(n-2) - 28*a(n-3) - 4*a(n-4)
          + 16*a(n-5) - 4*a(n-6).                                       */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the offset and small indices directly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)20;
    if (n == 3) return (seq_t)58;
    if (n == 4) return (seq_t)361;
    if (n == 5) return (seq_t)1588;
    if (n == 6) return (seq_t)7460;
    if (n == 7) return (seq_t)33136;
    if (n == 8) return (seq_t)146300;

    /* Initial six terms for the recurrence (indices 3..8). */
    seq_t a_nm6 = (seq_t)58;     /* a(3) */
    seq_t a_nm5 = (seq_t)361;    /* a(4) */
    seq_t a_nm4 = (seq_t)1588;   /* a(5) */
    seq_t a_nm3 = (seq_t)7460;   /* a(6) */
    seq_t a_nm2 = (seq_t)33136;  /* a(7) */
    seq_t a_nm1 = (seq_t)146300; /* a(8) */

    seq_t term = (seq_t)0;

    /* Compute forward using the 6‑term linear recurrence. */
    for (idx_t k = 9; k <= n; ++k) {
        term = (seq_t)6 * a_nm1
               - a_nm2
               - (seq_t)28 * a_nm3
               - (seq_t)4  * a_nm4
               + (seq_t)16 * a_nm5
               - (seq_t)4  * a_nm6;

        /* Shift the window of last six terms. */
        a_nm6 = a_nm5;
        a_nm5 = a_nm4;
        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = term;
    }

    return term;
}
