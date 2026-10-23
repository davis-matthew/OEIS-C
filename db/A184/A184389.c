/*
 * A184389  a(n) = Sum_{k=1..tau(n)} k, where tau is the number of divisors of
 *          n (A000005).
 * https://oeis.org/A184389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = τ(n)*(τ(n)+1)/2, where τ(n) is the number of divisors of
 *            n
 * Range:     u8 n <= 359, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;
    }
    size_t tau = divisor_count_u64((uint64_t)n);
    size_t val = tau * (tau + 1) / 2;
    return (seq_t)val;
}
