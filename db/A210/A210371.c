/*
 * A210371  Number of 2 X 2 matrices with all elements in {0,1,...,n} and
 *          nonnegative even determinant.
 * https://oeis.org/A210371
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (E^2 + O^2 + (2n+1)^2 + Σ_{k=1..n} floor(n/k)^2 * C_k)/2,
 *            where C_k = 1 for k = 1, else C_k = 2·φ(k); φ is Euler's totient
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 340, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* 1.  Odd/even counts for all pairs (a,d) */
    idx_t oddCnt = (n + 1) / 2;                  /* number of odd integers in [0,n]   */
    seq_t odd   = (seq_t)oddCnt * (seq_t)oddCnt; /* O = (#odd)^2                       */
    seq_t total = (seq_t)(n + 1) * (seq_t)(n + 1);/* T = (n+1)^2                        */
    seq_t even  = total - odd;                   /* E = T - O                           */
    seq_t evenSq = even * even;                  /* E²                                 */
    seq_t oddSq  = odd  * odd;                   /* O²                                 */
    seq_t EOsum  = evenSq + oddSq;               /* E² + O²                            */

    /* 2.  Contribution from zero product pairs */
    seq_t zeroCnt = (seq_t)2 * (seq_t)n + 1;      /* f[0] = 2n+1                         */
    seq_t zeroSq  = zeroCnt * zeroCnt;           /* (2n+1)²                             */

    /* 3.  Compute Euler's totient function up to n */
    if (n == 0) {
        /* All terms above are zero, a(0) = 1 */
        seq_t sumProd = 0;
        seq_t ssq = zeroSq + sumProd;
        return (EOsum + ssq) / 2;
    }

    unsigned int N = (unsigned int)n;            /* n <= 10000, fits in 32‑bit          */
    unsigned int *phi = (unsigned int*)malloc((N + 1) * sizeof(unsigned int));
    if (!phi) return 0;                          /* allocation failure: return 0        */
    for (unsigned int i = 0; i <= N; ++i) phi[i] = i;
    for (unsigned int p = 2; p <= N; ++p)
        if (phi[p] == p)                           /* p is prime */
            for (unsigned int m = p; m <= N; m += p)
                phi[m] -= phi[m] / p;

    /* 4.  Σ_{k=1..n} floor(n/k)^2 * C_k   with C_k = 1 for k=1, else 2·φ(k) */
    seq_t sumProd = 0;
    for (unsigned int k = 1; k <= N; ++k) {
        seq_t s  = (seq_t)n / (seq_t)k;           /* floor(n/k)                        */
        seq_t s2 = s * s;
        seq_t Ck = (k == 1 ? (seq_t)1 : (seq_t)2 * (seq_t)phi[k]);
        sumProd += s2 * Ck;
    }
    free(phi);

    /* 5.  Final combination */
    seq_t ssq = zeroSq + sumProd;                /* total sum of squares of f[p]       */
    seq_t result = (EOsum + ssq) / 2;
    return result;
}
