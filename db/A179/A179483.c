/*
 * A179483  A(k,3) where A(k,n) = Sum_{m=1..k} (-1)^(m+1) *binomial(n,m)*m^k.
 * https://oeis.org/A179483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 9, i32 n <= 19, i64 n <= 39, i128 n <= 80,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A179483
   Formula: a(n) = 3 - 3*2^n + 3^n   for n > 2
   Initial terms: a(1) = 3, a(2) = -9
*/

#include <stdint.h>
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)-9;
    seq_t pow2n = pow_seq((seq_t)2, (int)n);
    seq_t pow3n = pow_seq((seq_t)3, (int)n);
    return (seq_t)3 - (seq_t)3 * pow2n + pow3n;
}
