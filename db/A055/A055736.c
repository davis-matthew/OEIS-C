/*
 * A055736  Difference between number of prime factors of n and of phi(n).
 * https://oeis.org/A055736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A001221(n) - A001221(phi(n))
 * Range:     i8 n <= 5000, i16 n <= 5000, i32 n <= 5000, i64 n <= 5000,
 *            i128 n <= 5000, big any n
 * Helpers:   count_distinct_prime_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/count_distinct_prime_factors.h"
/*  A001221(n) = number of distinct prime factors of n */
/*  phi(n) = Euler's totient function */

static uint64_t totient_uint64(uint64_t n)
{
    if (n == 0) return 0;
    uint64_t result = n;
    if (n % 2 == 0) {
        result -= result / 2;
        while (n % 2 == 0) n /= 2;
    }
    for (uint64_t p = 3; p <= n / p; p += 2) {
        if (n % p == 0) {
            result -= result / p;
            while (n % p == 0) n /= p;
        }
    }
    if (n > 1) {
        result -= result / n;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* treat n==0 as 0 to avoid undefined use */
    if (n == 0) return 0;
    uint64_t num = (uint64_t)n;
    int cnt_n = count_distinct_prime_factors(num);
    uint64_t phi_n = totient_uint64(num);
    int cnt_phi = count_distinct_prime_factors(phi_n);
    return (seq_t)(cnt_n - cnt_phi);
}
