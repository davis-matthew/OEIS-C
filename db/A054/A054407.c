/*
 * A054407  Number of asymmetric n X n binary matrices under action of
 *          dihedral group of the square D_4.
 * https://oeis.org/A054407
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) =
 *            (1/8)*(2^(n^2)-2*2^((n^2+n)/2)-3*2^(n^2/2)+2*2^(n^2/4)+2*2^((n^2+2*n)/4))
 *            (n even) * a(n) =
 *            (1/8)*(2^(n^2)-4*2^((n^2+n)/2)-2^((n^2+1)/2)+4*2^((n^2+2*n+1)/4))
 *            (n odd) * Implements the formula given in OEIS entry A054407
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* For n = 0,1,2 the formula yields 0, but we handle them explicitly */
    if (n == 0 || n == 1 || n == 2)
        return 0;

    idx_t n2 = n * n;              /* n^2 (fits in idx_t for n up to 10k) */
    seq_t num;
    if ((n & 1) == 0) { /* n even */
        seq_t t1 = pow2(n2);                         /* 2^(n^2) */
        seq_t t2 = pow2((n2 + n) / 2);               /* 2^((n^2+n)/2) */
        seq_t t3 = pow2(n2 / 2);                     /* 2^(n^2/2) */
        seq_t t4 = pow2(n2 / 4);                     /* 2^(n^2/4) */
        seq_t t5 = pow2((n2 + 2 * n) / 4);           /* 2^((n^2+2*n)/4) */

        num = t1
              - ((seq_t)2 * t2)
              - ((seq_t)3 * t3)
              + ((seq_t)2 * t4)
              + ((seq_t)2 * t5);
    } else { /* n odd */
        seq_t t1 = pow2(n2);                         /* 2^(n^2) */
        seq_t t2 = pow2((n2 + n) / 2);               /* 2^((n^2+n)/2) */
        seq_t t3 = pow2((n2 + 1) / 2);               /* 2^((n^2+1)/2) */
        seq_t t4 = pow2((n2 + 2 * n + 1) / 4);       /* 2^((n^2+2*n+1)/4) */

        num = t1
              - ((seq_t)4 * t2)
              - t3
              + ((seq_t)4 * t4);
    }

    /* divide by 8 (exact division) */
    return num / ((seq_t)8);
}
