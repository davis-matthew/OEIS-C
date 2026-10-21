/*
 * A085037  Smallest square divisible by the n-th triangular number
 *          (n(n+1)/2).
 * https://oeis.org/A085037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the smallest square divisible by the n‑th triangular
 *            number T(n) = n·(n+1)/2. The square is obtained by multiplying
 *            T(n) by the product of the distinct primes that appear with an
 *            odd exponent in its factorization. This ensures all prime
 *            exponents in the final result are even
 * Range:     u8 n <= 5, u16 n <= 27, u32 n <= 363, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n >= 1 (offset 1).  For n = 0 the result is undefined. */
    /* Compute the triangular number T = n(n+1)/2.  It fits in 64 bits
       for all n used in the tests (n ≤ 10,000). */
    uint64_t nn = (uint64_t)n;
    uint64_t T = nn * (nn + 1) / 2;          /*  T ≤ 5·10^7 for n≤10,000 */

    seq_t multiplier = 1;                    /* product of primes with odd exponent */

    /* factor 2 specially */
    uint64_t temp = T;
    int exp = 0;
    while ((temp & 1ULL) == 0ULL) {          /* count exponent of 2 */
        temp >>= 1ULL;
        exp++;
    }
    if (exp & 1) multiplier *= (seq_t)2;     /* 2 appears odd number of times */

    /* factor odd primes */
    for (uint64_t p = 3ULL; p * p <= temp; p += 2ULL) {
        if (temp % p == 0ULL) {
            exp = 0;
            while (temp % p == 0ULL) {
                temp /= p;
                exp++;
            }
            if (exp & 1)                      /* odd exponent */
                multiplier *= (seq_t)p;
        }
    }

    if (temp > 1ULL) {                       /* remaining prime factor */
        /* its exponent is 1 (odd) */
        multiplier *= (seq_t)temp;
    }

    /* the answer is T multiplied by the product of primes with odd
       exponents, producing a square multiple of T. */
    return (seq_t)T * multiplier;
}
