/*
 * A214799  Let S be a set of n positive numbers such that all n choose 2
 *          pairwise GCD's are distinct, and let max(S) denote the largest
 *          element of S; a(n) is the minimal value of max(S) over all choices
 *          for S.
 * https://oeis.org/A214799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=2, a(3)=6, a(4)=18, a(5)=54; for n>=6,
 *            a(n)=15*2^(n-3)
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, return 0 */
    if (n == 1) return 1;
    if (n == 2) return 2;
    if (n == 3) return 6;
    if (n == 4) return 18;
    if (n == 5) return 54;
    seq_t two_pow = pow2((idx_t)(n - 3));  /* 2^(n-3) */
    return (seq_t)15 * two_pow;           /* 15 * 2^(n-3) */
}
