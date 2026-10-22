/*
 * A005772  Number of permutations of length n with 2 cycle lengths.
 * https://oeis.org/A005772
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{1<=c1<c2} Sum_{m1,m2>=1, m1*c1+m2*c2=n}
 *            n!/(c1^m1*m1!*c2^m2*m2!), computed as Sum_{c2} Sum_{k=m*c2}
 *            binomial(n,k)*T(c2,m)*S_{c2}(n-k), where T(c,m) = (cm)!/(c^m*m!)
 *            = T(c,m-1)*Prod_{i=(m-1)c+1}^{mc-1} i is the number of
 *            permutations of cm elements whose cycles all have length c, and
 *            S...
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/alloc_zero.h"
#include "../utils/release.h"

seq_t a(idx_t n) {
    if (n < 3) return 0;

    seq_t *C = alloc_zero(n + 1);   /* binomial(n, k) */
    seq_t *S = alloc_zero(n + 1);   /* S[j] = sum of T(c1, j/c1) over c1 < c, c1 | j */
    seq_t *tv = alloc_zero(n + 1);  /* T(c, m) for the current c */

    C[0] = 1;
    for (idx_t k = 1; k <= n; k++)
        C[k] = C[k - 1] * (seq_t)(n - k + 1) / (seq_t)k;

    seq_t result = 0;
    for (idx_t c = 1; c <= n; c++) {
        seq_t t = 1;
        idx_t m = 0;
        for (idx_t k = c; k <= n; k += c) {
            m++;
            for (idx_t i = k - c + 1; i < k; i++)
                t *= (seq_t)i;
            tv[m] = t;
            if (k < n)
                result += C[k] * t * S[n - k];
        }
        for (idx_t j = 1; j <= m; j++)
            S[j * c] += tv[j];
    }

    release(C, n + 1);
    release(S, n + 1);
    release(tv, n + 1);
    return result;
}
