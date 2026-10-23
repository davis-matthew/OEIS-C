/*
 * A181497  a(n) is the smallest m such that A056753(m) = 2*n + 1.
 * https://oeis.org/A181497
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = Sum_{k=1..n} 2^ceiling(log_2(k))
 * Range:     u8 n <= 18, u16 n <= 298, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* compute m = floor(log2(n)) */
    idx_t m = 0;
    idx_t tmp = n;
    while (tmp > 1) {
        tmp >>= 1;
        ++m;
    }

    seq_t sum = 0;

    /* full blocks for m = 0 .. m-1: 2^(2*m+1) - 2^m */
    for (idx_t i = 0; i < m; ++i) {
        seq_t term_full = pow2(2 * i + 1) - pow2(i);
        sum += term_full;
    }

    /* last (possibly partial) block */
    idx_t start = ((idx_t)1) << m;                  /* 2^m    */
    idx_t cnt   = n - start + 1;                    /* number of terms in last block */
    seq_t t2m   = pow2(m);                          /* 2^m    */
    seq_t t2m1  = pow2(m + 1);                      /* 2^(m+1) */
    seq_t partial = t2m + ((seq_t)(cnt - 1)) * t2m1;
    sum += partial;

    return sum;
}
