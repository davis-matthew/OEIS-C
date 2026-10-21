/*
 * A226881  Number of n-length binary words w with #(w,0) >= #(w,1) >= 1,
 *          where #(w,x) gives the number of digits x in w.
 * https://oeis.org/A226881
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..⌊n/2⌋} binomial(n,i)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (equivalently: a(n)=2^(n-1)-1 for odd n, and 2^(n-1)-1+binomial(n,n/2)/2 for even n) */

#include "../utils/pow2.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* a(0)=a(1)=0 */

    seq_t pow = pow2(n - 1);      /* 2^(n-1) */
    seq_t res = pow - 1;          /* 2^(n-1)-1 */

    if ((n & 1ULL) == 0) {        /* n even: add binomial(n,n/2)/2 */
        seq_t bin = binomial(n, n / 2);
        res += bin / 2;
    }

    return res;
}
