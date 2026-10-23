/*
 * A179504  a(n) = sigma(2n,n) for n>0 with a(0)=1.
 * https://oeis.org/A179504
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(2*n , n) (sum of the n-th powers of the divisors of
 *            2n)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   seq_one, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses the multiplicative property of σ_k(n) and
   computes each prime power contribution as 1 + p^n + p^{2n} + … + p^{e n}
   avoiding intermediate overflows by accumulating the sum iteratively. */

/* required utility headers */
#include <stdint.h>
#include "../utils/seq_one.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return seq_one();            /* a(0) = 1 */

    /* factor m = 2*n */
    idx_t m = 2 * n;
    idx_t temp = m;
    seq_t result = seq_one();

    /* handle factor 2 separately (exponent e = 0 or 1) */
    if ((temp & 1) == 0) {            /* temp even */
        idx_t e = 0;
        while ((temp & 1) == 0) {
            temp >>= 1;
            ++e;
        }
        seq_t pk = seq_pow((seq_t)2, n);   /* 2^n */
        seq_t sum = seq_one();             /* 1 */
        seq_t term = seq_one();            /* 1 */
        for (idx_t i = 1; i <= e; ++i) {
            term = term * pk;              /* multiply by p^n */
            sum += term;                   /* accumulate the sum */
        }
        result *= sum;
    }

    /* odd prime factors */
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            seq_t pk = seq_pow((seq_t)p, n); /* p^n */
            seq_t sum = seq_one();          /* 1 */
            seq_t term = seq_one();         /* 1 */
            for (idx_t i = 1; i <= e; ++i) {
                term = term * pk;
                sum += term;
            }
            result *= sum;
        }
    }

    /* remaining prime factor > sqrt(temp), if any */
    if (temp > 1) {
        /* exponent e is 1 */
        seq_t pk = seq_pow((seq_t)temp, n); /* prime^n */
        seq_t sum = seq_one() + pk;         /* 1 + p^n */
        result *= sum;
    }

    return result;
}
