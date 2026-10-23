/*
 * A018218  Sum(C(j)*(n-j)*4^(n-j-1),j=0..n-1), C = Catalan numbers.
 * https://oeis.org/A018218
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)*(4^n - binomial(2n+1, n))/2
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   binomial, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow4.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* 4^n */
    seq_t pow4n = pow4(n);
    /* binomial(2n+1, n) */
    seq_t bin = binomial(2 * n + 1, n);
    /* difference */
    seq_t diff = pow4n - bin;

    /* avoid overflow: divide first, using that
       either (n+1) or diff is even */
    if ((n + 1) & 1) {          /* n+1 is odd, diff is even */
        diff /= 2;
        return diff * (n + 1);
    } else {                     /* n+1 is even */
        return ((n + 1) / 2) * diff;
    }
}
