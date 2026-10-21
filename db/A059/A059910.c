/*
 * A059910  a(n) = |{m : multiplicative order of n mod m = 5}|.
 * https://oeis.org/A059910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = τ(n^5-1) – τ(n-1) (OEIS A059910)
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"   /* factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt) */

seq_t a(idx_t n)
{
    /* a(1) = 0 by definition */
    if (n == 1)
        return (seq_t)0;

    uint64_t nn = (uint64_t)n;

    /* Factor n-1 */
    uint64_t primes1[64], exps1[64];
    size_t cnt1;
    factor_u64(nn - 1U, primes1, exps1, &cnt1);

    /* Compute τ(n-1) */
    uint64_t tau_n_minus1 = 1;
    for (size_t i = 0; i < cnt1; ++i)
        tau_n_minus1 *= (exps1[i] + 1U);

    /* Compute Φ5(n) = n^4 + n^3 + n^2 + n + 1  (fits in 64 bits for n <= 10000) */
    uint64_t n2 = nn * nn;              /* n²  <= 1e8      */
    uint64_t n3 = n2 * nn;              /* n³  <= 1e12     */
    uint64_t n4 = n3 * nn;              /* n⁴  <= 1e16     */
    uint64_t phi5 = n4 + n3 + n2 + nn + 1U;

    /* Factor Φ5(n) */
    uint64_t primes2[64], exps2[64];
    size_t cnt2;
    factor_u64(phi5, primes2, exps2, &cnt2);

    /* Merge factorisations to compute τ(n^5-1) = τ((n-1)·Φ5(n)) */
    uint64_t tau_n5_minus1 = 1;
    size_t i = 0, j = 0;
    while (i < cnt1 && j < cnt2) {
        if (primes1[i] < primes2[j]) {
            tau_n5_minus1 *= (exps1[i] + 1U);
            ++i;
        } else if (primes1[i] > primes2[j]) {
            tau_n5_minus1 *= (exps2[j] + 1U);
            ++j;
        } else {                           /* common prime (only possibility is 5) */
            uint64_t exp = exps1[i] + exps2[j];
            tau_n5_minus1 *= (exp + 1U);
            ++i; ++j;
        }
    }
    while (i < cnt1) {
        tau_n5_minus1 *= (exps1[i] + 1U);
        ++i;
    }
    while (j < cnt2) {
        tau_n5_minus1 *= (exps2[j] + 1U);
        ++j;
    }

    /* a(n) = τ(n^5-1) – τ(n-1) */
    uint64_t result = tau_n5_minus1 - tau_n_minus1;
    return (seq_t)result;
}
