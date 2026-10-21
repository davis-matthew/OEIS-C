/*
 * A076887  Sum of divisors of nonzero palindromic numbers.
 * https://oeis.org/A076887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, look
 * Formula:   a(n) = A000203(A002113(n+1))
 * Range:     u8 n <= 25, u16 n <= 330, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   nth_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/nth_palindrome.h"

/* Compute sum of divisors σ(n) for n ≤ 10^12 (fits in uint64_t). */
static uint64_t sigma_uint64(uint64_t n) {
    uint64_t res = 1;
    uint64_t m = n;

    /* factor 2 */
    if ((m & 1ULL) == 0) {
        uint64_t sum = 1, pow = 1;
        while ((m & 1ULL) == 0) {
            m >>= 1;
            pow <<= 1;
            sum += pow;
        }
        res *= sum;
    }

    /* factor odd primes */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            uint64_t sum = 1, pow = 1;
            while (m % p == 0) {
                m /= p;
                pow *= p;
                sum += pow;
            }
            res *= sum;
        }
    }

    if (m > 1)          /* remaining prime factor */
        res *= (1 + m);

    return res;
}

seq_t a(idx_t n) {
    uint64_t pal = nth_palindrome(n);
    uint64_t sigma_val = sigma_uint64(pal);
    return (seq_t)sigma_val;
}
