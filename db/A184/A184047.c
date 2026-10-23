/*
 * A184047  1/9 the number of (n+1) X 9 0..2 arrays with all 2 X 2 subblocks
 *          having the same four values.
 * https://oeis.org/A184047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = 3*2^(n/2) + 2^(n+1) + 553 for n even * a(n) = 2^(n+1) +
 *            2^((n+3)/2) + 553 for n odd * (Offset 1) a(1)=561, a(2)=567, …
 * Range:     u8 none, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* For n < 1 the offset is 1; return 0 gracefully. */
    if (n == 0)
        return (seq_t)0;

    seq_t term1 = pow2(n + 1);       /* 2^(n+1) */
    seq_t term2;
    seq_t result;

    if ((n & 1) == 0) {              /* n even */
        term2 = pow2(n / 2);         /* 2^(n/2) */
        result = term1 + (seq_t)3 * term2 + (seq_t)553;
    } else {                         /* n odd */
        term2 = pow2((n + 3) / 2);   /* 2^((n+3)/2) */
        result = term1 + term2 + (seq_t)553;
    }

    return result;
}
