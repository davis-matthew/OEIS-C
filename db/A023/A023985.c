/*
 * A023985  Sum of exponents in prime-power factorization of multinomial
 *          coefficient M(6n,2n,2n,2n).
 * https://oeis.org/A023985
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A022559(6*n) - 3*A022559(2*n) A022559(k) = sum_{i=2..k}
 *            Ω(i), the total number of prime factors (with multiplicity) of
 *            all integers up to k. The implemented routine computes the
 *            required cumulative sums on the fly, with a simple sieve for
 *            smallest prime factors
 * Range:     u8 n <= 213, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* External linkage as required */
seq_t a(idx_t n)
{
    idx_t m6 = 6 * n;                /* upper index for cumulative sum */
    idx_t m2 = 2 * n;                /* for the subtracted part          */

    /* Allocate arrays up to m6.
       The data size is tiny (m6 ≤ 6·10⁴ for requested limits).      */
    size_t sz = (size_t)(m6 + 1);
    uint32_t *spf  = (uint32_t*)malloc(sz * sizeof(uint32_t));
    if (!spf) return 0;             /* allocation failed */
    for (size_t i = 0; i < sz; ++i) spf[i] = 0;
    if (sz > 1) spf[1] = 1;          /* 1 is not prime but convenient */

    /* Sieve for smallest prime factor. */
    for (uint32_t i = 2; i <= m6; ++i) {
        if (spf[i] == 0) {          /* i is prime */
            spf[i] = i;
            uint64_t ii = (uint64_t)i * (uint64_t)i;
            if (ii <= m6) {
                for (uint32_t j = i * i; j <= m6; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
    }

    /* Compute Ω(i) using the recurrence Ω(i) = Ω(i / spf[i]) + 1.  */
    uint8_t *omega = (uint8_t*)malloc(sz * sizeof(uint8_t));
    if (!omega) { free(spf); return 0; }
    omega[0] = 0;
    if (sz > 1) omega[1] = 0;
    for (uint32_t i = 2; i <= m6; ++i)
        omega[i] = omega[i / spf[i]] + 1;

    /* Cumulative sums up to m2 and m6.  */
    seq_t cum = 0;
    seq_t cum2 = 0;
    for (uint32_t i = 1; i <= m6; ++i) {
        if (i >= 2) cum += (seq_t)omega[i];
        if (i == m2) cum2 = cum;
    }

    seq_t result = cum - 3 * cum2;

    free(spf);
    free(omega);
    return result;
}
