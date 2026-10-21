/*
 * A054185  Binomial transform of A000031.
 * https://oeis.org/A054185
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A054185: Binomial transform of A000031.
 * Formula: a(0)=1, for n>0
 *   a(n) = Σ_{k=0..n} C(n,k) * b(k)
 *   where b(k) = (1/k) * Σ_{d|k} φ(d) * 2^{k/d}   (b(0)=1)
 *   φ denotes Euler's totient function.
 *
 * The implementation precomputes φ up to n, then evaluates the binomial
 * transform in O(n²) time the divisor loop for each k contributing
 * at most O(√k) operations.  For n≤10000 this runs well within the
 * time limits.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Allocate array for Euler's totient values 0..n. */
    uint64_t *phi = (uint64_t *)malloc((size_t)(n + 1) * sizeof(uint64_t));
    if (!phi) return (seq_t)0;  /* OOM -> return 0 (unlikely in tests) */

    /* Initial totient values: φ(i)=i. */
    for (size_t i = 0; i <= (size_t)n; ++i) phi[i] = i;

    /* Sieve totient. */
    for (size_t p = 2; p <= (size_t)n; ++p) {
        if (phi[p] == p) {           /* p is prime */
            for (size_t j = p; j <= (size_t)n; j += p)
                phi[j] -= phi[j] / p;
        }
    }

    /* Binomial transform: a(n)=Σ_{k=0..n} C(n,k)*b(k). */
    seq_t ans   = (seq_t)1;   /* term for k=0: C(n,0)*b(0)=1*1 */
    seq_t binom = (seq_t)1;   /* C(n,0)   */

    for (idx_t k = 1; k <= n; ++k) {
        /* Update binomial coefficient: C(n,k)=C(n,k-1)*(n-k+1)/k. */
        binom = binom * (seq_t)(n - k + 1);
        binom = binom / (seq_t)k;

        /* Compute b(k)= (1/k) * Σ_{d|k} φ(d) * 2^{k/d}. */
        seq_t sum = (seq_t)0;
        for (idx_t d = 1; d * d <= k; ++d) {
            if (k % d == 0) {
                idx_t m = k / d;              /* m = k/d */
                seq_t term = pow_uint(2ULL, m); /* 2^{k/d} */
                term *= (seq_t)phi[d];         /* φ(d) * 2^{m} */
                sum += term;

                if (d != m) {                 /* other divisor pair */
                    term = pow_uint(2ULL, d);  /* 2^{d} */
                    term *= (seq_t)phi[m];    /* φ(m) * 2^{d} */
                    sum += term;
                }
            }
        }
        seq_t bk = sum / (seq_t)k;

        /* Add contribution to the final sum. */
        ans += binom * bk;
    }

    free(phi);
    return ans;
}
