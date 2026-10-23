/*
 * A130783  Maximum value of the n-th difference of a permutation of 0..n.
 * https://oeis.org/A130783
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(2^(n-1)-binomial(n-1,n/2)) if n even; a(n) =
 *            ((n+1)/2)*(2^n - binomial(n,(n+1)/2)) if n odd
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 59, u128 n <= 122,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"  /* provides binomial() */
#include "../utils/pow2.h"      /* provides pow2() */

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    if ((n & 1) == 0) {               /* n is even */
        seq_t pow = pow2(n - 1);                          /* 2^(n-1) */
        seq_t bin = binomial(n - 1, n >> 1);              /* C(n-1, n/2) */
        seq_t diff = pow - bin;                           /* 2^(n-1) - bin */
        seq_t factor = (seq_t)(n + 1);                    /* n+1 */
        return factor * diff;
    } else {                                 /* n is odd */
        seq_t pow = pow2(n);                                 /* 2^n */
        seq_t bin = binomial(n, (n + 1) >> 1);               /* C(n, (n+1)/2) */
        seq_t diff = pow - bin;                              /* 2^n - bin */
        seq_t factor = (seq_t)((n + 1) / 2);                  /* (n+1)/2 */
        return factor * diff;
    }
}
