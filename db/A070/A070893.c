/*
 * A070893  Let r, s, t be three permutations of the set {1,2,3,..,n}; a(n) =
 *          value of Sum_{i=1..n} r(i)*s(i)*t(i), with r={1,2,3,..,n};
 *          s={n,n-1,..,1} and t={n,n-2,n-4,...,1,...,n-3,n-1}.
 * https://oeis.org/A070893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/96)*(2*n*(n+2)*(3*n^2+10*n+4)+3*(-1)^n-3) (OEIS
 *            A070893)
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 468, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* for safety; OEIS offset is 1 */

    seq_t sn = (seq_t)n;

    /* Compute 2*n*(n+2)*(3*n^2+10*n+4) */
    seq_t term1 = sn * ((seq_t)(n + 2));                     /* n*(n+2) */
    seq_t term2 = (seq_t)3 * sn * sn;                         /* 3*n^2 */
    seq_t term3 = (seq_t)10 * sn;                             /* 10*n */
    seq_t inner  = term2 + term3 + (seq_t)4;                  /* 3*n^2+10*n+4 */
    seq_t prod   = term1 * inner;                             /* n*(n+2)*(...) */
    seq_t numer  = (seq_t)2 * prod;                           /* 2*prod */

    /* Adjust for (-1)^n term: +3*(-1)^n-3
       Even n: 3*1-3 = 0
       Odd  n: 3*(-1)-3 = -6      */
    if (n & 1) {
        numer = numer - (seq_t)6;
    }

    /* final division by 96 */
    return numer / (seq_t)96;
}
