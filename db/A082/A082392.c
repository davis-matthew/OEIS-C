/*
 * A082392  Expansion of (1/x) * Sum_{k>=0} x^2^k / (1 - 2*x^2^(k+1)).
 * https://oeis.org/A082392
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(2*n) = 2^n, a(2*n+1) = a(n)
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 63, u64 n <= 127,
 *            u128 n <= 255, big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow_int.h"

/* Compute A082392: a(n) = 1 for n=0; otherwise
 * if n is even, a(n)=2^(n/2); if n is odd, a(n)=a((n-1)/2).
 * The function is recursive and works for any index n>=0.
 */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    if ((n & 1) == 0) {  /* even */
        idx_t e = n >> 1;
        return pow_int((seq_t)2, e);
    } else {            /* odd */
        return a((n - 1) >> 1);
    }
}
