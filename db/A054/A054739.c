/*
 * A054739  Number of inequivalent n X n matrices over GF(3) under action of
 *          dihedral group of the square D_4.
 * https://oeis.org/A054739
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1/8 * (3^(n^2) + 2*3^(n^2/4) + 3*3^(n^2/2) +
 *            2*3^((n^2+n)/2)) if n even 1/8 * (3^(n^2) + 2*3^((n^2+3)/4) +
 *            3^((n^2+1)/2) + 4*3^((n^2+n)/2)) if n odd (OEIS A054739 –
 *            matrices over GF(3) modulo the dihedral group D4)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t sq = (seq_t)n * (seq_t)n;          /* n^2 */
    seq_t term = big_pow((seq_t)3, sq);      /* 3^(n^2) */
    seq_t sum = term;

    if ((n & 1) == 0) {                     /* n is even */
        seq_t e2 = sq / 4;                   /* n^2/4 */
        seq_t t2 = big_pow((seq_t)3, e2);
        sum += 2 * t2;                       /* 2*3^(n^2/4) */

        seq_t e3 = sq / 2;                   /* n^2/2 */
        seq_t t3 = big_pow((seq_t)3, e3);
        sum += 3 * t3;                       /* 3*3^(n^2/2) */

        seq_t e4 = (sq + n) / 2;             /* (n^2+n)/2 */
        seq_t t4 = big_pow((seq_t)3, e4);
        sum += 2 * t4;                       /* 2*3^((n^2+n)/2) */
    } else {                                 /* n is odd */
        seq_t e2 = (sq + 3) / 4;             /* (n^2+3)/4 */
        seq_t t2 = big_pow((seq_t)3, e2);
        sum += 2 * t2;                       /* 2*3^((n^2+3)/4) */

        seq_t e3 = (sq + 1) / 2;             /* (n^2+1)/2 */
        seq_t t3 = big_pow((seq_t)3, e3);
        sum += t3;                           /* 3^((n^2+1)/2) */

        seq_t e4 = (sq + n) / 2;             /* (n^2+n)/2 */
        seq_t t4 = big_pow((seq_t)3, e4);
        sum += 4 * t4;                       /* 4*3^((n^2+n)/2) */
    }

    return sum / 8;                           /* divide by 8 */
}
