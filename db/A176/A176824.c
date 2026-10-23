/*
 * A176824  a(n) = (n+1)^n mod n^n.
 * https://oeis.org/A176824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)^n mod n^n (equivalently (n+1)^n - 2*n^n)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Compute n^n and (n+1)^n.  The result is (n+1)^n - 2*n^n. */
    seq_t pow_n   = seq_pow((seq_t)n, n);          /* n^n               */
    seq_t pow_n2  = pow_n;                         /* copy for doubling */
    pow_n2 += pow_n;                               /* 2*n^n             */
    seq_t pow_np1 = seq_pow((seq_t)(n + 1), n);    /* (n+1)^n           */
    return pow_np1 - pow_n2;                       /* (n+1)^n - 2*n^n   */
}
