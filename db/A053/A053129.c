/*
 * A053129  Binomial coefficients C(2*n-6,7).
 * https://oeis.org/A053129
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(2*n-6, 7) for n >= 7, else 0
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 44, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  // for uint64_t type in case needed by seq_t/idx_t

seq_t a(idx_t n)
{
    /* Return 0 for indices before the offset. */
    if (n < 7) {
        return 0;
    }

    /* m = 2*n - 6;  we will compute C(m, 7). */
    idx_t m = 2 * n - 6;

    /* Prime factorization of 7! = 5040 = 2^4 * 3^2 * 5 * 7 */
    unsigned char rem_cnt[4] = {4, 2, 1, 1};  /* counters for primes 2,3,5,7 respectively */
    const unsigned int primes[4] = {2, 3, 5, 7};

    seq_t res = 1;

    /* Multiply numerators and divide out denominators whenever possible
       to keep intermediate values within seq_t bounds. */
    for (idx_t i = 1; i <= 7; ++i) {
        seq_t term = (seq_t)(m - i + 1);  /* term of the numerator */
        res *= term;

        /* Attempt to cancel the prime factors in the denominator. */
        for (int p = 0; p < 4; ++p) {
            while (rem_cnt[p] > 0 && (res % (seq_t)primes[p] == 0)) {
                res /= (seq_t)primes[p];
                --rem_cnt[p];
            }
        }
    }

    /* All factors of 7! should have been cancelled. */
    return res;
}
