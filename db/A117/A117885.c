/*
 * A117885  Numbers k such that a regular (k^3-k)-gon can be constructed by
 *          means of a ruler and compass.
 * https://oeis.org/A117885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   big_pow, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A117885: 2,3,4,5,16,256,65536,...
   a(1)=2, a(2)=3, a(3)=4, a(4)=5.
   For n≥5: a(n) = 2^(2^(n-3)).
   This follows from the characterization of constructible
   (k^3-k)-gons: the only admissible k are 3,5 and k=2^(2^m)
   for m≥0. */
#include "../utils/pow2.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)4;
    if (n == 4) return (seq_t)5;

    /* n >= 5 */
    idx_t m = n - 3;            /* exponent for the inner power of two  */
    seq_t inner = pow2(m);      /* compute 2^m                             */
    seq_t result = big_pow((seq_t)2, inner);  /* 2^(2^m) = 2^(2^(n-3)) */
    return result;
}
