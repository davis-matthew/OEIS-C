/*
 * A031972  a(n) = Sum_{k=1..n} n^k.
 * https://oeis.org/A031972
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} n^k = n*(n^n-1)/(n-1) for n>1. a(0) = 0 ,
 *            a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    seq_t nt   = (seq_t)n;                 /* convert index to seq_t */
    seq_t n_pow_n = big_pow(nt, nt);       /* compute n^n      */
    seq_t numer = nt * (n_pow_n - (seq_t)1); /* n*(n^n-1)      */
    seq_t denom = nt - (seq_t)1;            /* n-1            */
    return numer / denom;                  /* integer division (exact) */
}
