/*
 * A129628  Inverse Moebius transform of A001511.
 * https://oeis.org/A129628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = tau(odd_part(n)) * (k+1)(k+2)/2 where k is the exponent
 *            of 2 in the prime factorization of n. This follows from the
 *            recurrence a(2n) = a(n)+tau(2n) and a(2n+1)=tau(2n+1) together
 *            with the multiplicativity of tau
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    idx_t temp = n;
    idx_t k = 0;
    while ((temp & 1ULL) == 0) {
        ++k;
        temp >>= 1;
    }
    seq_t tau = (seq_t)divisor_count_u64(temp);
    seq_t factor = (seq_t)(k + 1) * (seq_t)(k + 2) / 2;
    return tau * factor;
}
