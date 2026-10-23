/*
 * A135836  Column three of the triangular matrix in A135835.
 * https://oeis.org/A135836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 26, u64 n <= 58, u128 n <= 122,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Approximation formulas for A135836 (see OEIS entry A135836):
      a(2k)   = (1/2)*(55 + 26*k + 2^(n+7) - 61*3^(k+1))
      a(2k+1) = (1/2)*(68 + 26*k + 2^(n+7) -106*3^(k+1))
   where n = 2k or 2k+1 respectively.
   A linear recurrence of order 5 obtained from the g.f. produces the
   same values:
      a(n) = 4*a(n-1) - 2*a(n-2) -10*a(n-3) +15*a(n-4) -6*a(n-5)    (n≥6)
*/

#include "../utils/seq_mul.h"

seq_t a(idx_t n) {
    /* initial five terms a(1) … a(5) */
    static const seq_t init[5] = {
        (seq_t)3,  (seq_t)22, (seq_t)82, (seq_t)254, (seq_t)677
    };

    if (n <= 5) return init[n-1];

    seq_t buf[5];
    for (idx_t i = 0; i < 5; ++i) buf[i] = init[i];

    for (idx_t i = 6; i <= n; ++i) {
        /* compute next term by the 5‑term linear recurrence */
        seq_t t1 = seq_mul(buf[4], (seq_t)4);   /* 4*a(n-1)   */
        seq_t t2 = seq_mul(buf[3], (seq_t)2);   /* 2*a(n-2)   */
        seq_t t3 = seq_mul(buf[2], (seq_t)10);  /* 10*a(n-3)  */
        seq_t t4 = seq_mul(buf[1], (seq_t)15);  /* 15*a(n-4)  */
        seq_t t5 = seq_mul(buf[0], (seq_t)6);   /* 6*a(n-5)   */

        seq_t left  = t1 + t4;                      /* 4*a(n-1) + 15*a(n-4) */
        seq_t right = t2 + t3 + t5;                 /* 2*a(n-2) +10*a(n-3)+6*a(n-5) */
        seq_t next  = left - right;                 /* a(n)          */

        /* shift window */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = next;
    }

    return buf[4];
}
