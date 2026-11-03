/*
 * A188385  Highest exponent in the prime factorization of n^n.
 * https://oeis.org/A188385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * A051903(n), where A051903(n) = max exponent e with
 *            p^e dividing n (i.e., the largest prime exponent in the
 *            factorization of n). This is equivalent to the highest exponent
 *            in the prime factorization of n^n
 * Range:     u8 n <= 63, u16 n <= 6143, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 80 terms through n = 9267 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t _max_prime_exponent(uint64_t n) {
    uint64_t max_exp = 0;
    uint64_t tmp = n;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            uint64_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            if (exp > max_exp)
                max_exp = exp;
        }
    }
    /* Remaining prime factor, if any, contributes exponent 1. */
    if (tmp > 1 && 1 > max_exp)
        max_exp = 1;
    return max_exp;
}

seq_t a(idx_t n) {
    if (n < 1)
        return (seq_t)0;              /* undefined for n < 1, return 0 as safe default */

    uint64_t exp = _max_prime_exponent((uint64_t)n);
    return (seq_t)n * (seq_t)exp;      /* multiply in seq_t to avoid overflow in intermediate */
}
