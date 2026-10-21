/*
 * A086275  Number of distinct Gaussian primes in the factorization of n.
 * https://oeis.org/A086275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A059841(n) + 2*A005089(n) + A005091(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A059841(n) = 1 if n is even, 0 otherwise
   A005089(n) = number of distinct prime factors of n that are ≡1 (mod 4)
   A005091(n) = number of distinct prime factors of n that are ≡3 (mod 4) */

#include <stdint.h>
#include "../utils/factor_uint64.h"   /* provides factor_uint64() */

seq_t a(idx_t n)
{
    if (n < 2)               /* a(1) = 0, and values for n < 1 are irrelevant */
        return 0;

    uint64_t nn = (uint64_t)n;

    /* factorization: fac[i] holds a distinct prime factor, exp[i] its exponent,
       cnt is the number of distinct prime factors. */
    uint64_t fac[20];
    int exp[20];
    int cnt = 0;
    factor_uint64(nn, fac, exp, &cnt);

    int count1 = 0;   /* primes ≡ 1 (mod 4) */
    int count3 = 0;   /* primes ≡ 3 (mod 4) */
    int even  = (nn & 1ULL) == 0 ? 1 : 0;   /* A059841 */

    for (int i = 0; i < cnt; ++i) {
        uint64_t p = fac[i];
        if (p == 2)           /* 2 contributes only to the even flag */
            continue;
        if ((p & 3ULL) == 1)
            ++count1;
        else if ((p & 3ULL) == 3)
            ++count3;
    }

    seq_t res = (seq_t)count3
              + (seq_t)2 * (seq_t)count1
              + (seq_t)even;
    return res;
}
