/*
 * A064589  a(n) = (3^n)^12*((3^n)^2-1)*((3^n)^6-1)*((3^n)^8 + (3^n)^4+1).
 * https://oeis.org/A064589
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3^n)^12 * ((3^n)^2-1) * ((3^n)^6-1) * ((3^n)^8 + (3^n)^4
 *            + 1)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 0, u64 n <= 1, u128 n <= 2,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* t = 3^n */
    seq_t t   = big_pow((seq_t)3, (seq_t)n);

    /* auxiliary powers of t */
    seq_t t2  = t * t;          /* 3^(2n) */
    seq_t t4  = t2 * t2;        /* 3^(4n) */
    seq_t t6  = t4 * t2;        /* 3^(6n) */
    seq_t t8  = t4 * t4;        /* 3^(8n) */
    seq_t t12 = t6 * t6;        /* 3^(12n) */

    seq_t res = t12;
    res *= (t2 - ((seq_t)1));
    res *= (t6 - ((seq_t)1));
    res *= (t8 + t4 + ((seq_t)1));

    return res;
}
