/*
 * A175038  In the sequence of positive integers A000027, number of digits
 *          between successive primes.
 * https://oeis.org/A175038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = sum_{i=prime(n)+1}^{prime(n+1)-1} digits(i)
 * Range:     u8 n <= 3384, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_count, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/digit_count.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    uint32_t p = get_prime(n);
    uint32_t q = get_prime(n + 1);

    seq_t sum = (seq_t)0;
    for (uint64_t i = (uint64_t)p + 1; i < (uint64_t)q; ++i)
    {
        sum += (seq_t)digit_count(i);
    }
    return sum;
}
