/*
 * A145340  a(n) = the maximum of d(p(n)-1) and d(p(n)+1), where d(m) is the
 *          number of divisors of m and p(n) is the n-th prime.
 * https://oeis.org/A145340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max(d(p(n)-1), d(p(n)+1)) where d(m) is the divisor count
 *            of m and p(n) is the n-th prime
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   divisor_count_u64, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    /* n is 1-based. */
    uint32_t prime = get_prime(n);
    uint64_t p = (uint64_t)prime;

    uint64_t d1 = divisor_count_u64(p - 1);
    uint64_t d2 = divisor_count_u64(p + 1);

    uint64_t result = d1 > d2 ? d1 : d2;

    return (seq_t)result;
}
