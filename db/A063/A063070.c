/*
 * A063070  a(n) = phi(n) - d(n), where d(n) is the number of divisors
 *          function (A000005).
 * https://oeis.org/A063070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = phi(n) - d(n) (Euler's totient minus number of divisors)
 * Range:     i8 n <= 129, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t phi_uint64(uint64_t x)
{
    if (x <= 1) return (seq_t)1;
    seq_t result = (seq_t)x;
    uint64_t temp = x;
    for (uint64_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            while (temp % p == 0) temp /= p;
            result -= result / (seq_t)p;
        }
    }
    if (temp > 1) {
        result -= result / (seq_t)temp;
    }
    return result;
}

static seq_t divisor_count_uint64(uint64_t x)
{
    seq_t count = 1;
    uint64_t temp = x;
    for (uint64_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            uint64_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                exp++;
            }
            count *= (seq_t)(exp + 1);
        }
    }
    if (temp > 1) count *= 2;
    return count;
}

seq_t a(idx_t n)
{
    uint64_t u = (uint64_t)n;
    seq_t phi = phi_uint64(u);
    seq_t d   = divisor_count_uint64(u);
    return phi - d;
}
