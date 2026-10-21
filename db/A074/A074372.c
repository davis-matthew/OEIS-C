/*
 * A074372  1 + the sum of the distinct primes dividing n.
 * https://oeis.org/A074372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 + sum of distinct primes dividing n
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   factor_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the helper function factor_uint64 to obtain the prime factorisation of n */
#include "../utils/factor_uint64.h"

seq_t a(idx_t n)
{
    if (n < 2) {
        return 1;                     /* a(1) = 1, and a(0) is irrelevant */
    }

    uint64_t nn = (uint64_t)n;         /* idx_t is uint64_t, but cast for clarity */

    uint64_t fac[20];                  /* primes */
    int      exp[20];                  /* exponents (unused) */
    int      cnt;                      /* number of distinct primes */

    factor_uint64(nn, fac, exp, &cnt); /* factorise nn */

    seq_t sum = 0;
    for (int i = 0; i < cnt; ++i) {
        sum += (seq_t)fac[i];
    }
    sum += (seq_t)1;                  /* add the constant term */

    return sum;
}
