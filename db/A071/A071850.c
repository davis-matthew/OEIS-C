/*
 * A071850  Smallest k > n such that n divides prime(k) - prime(n).
 * https://oeis.org/A071850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = smallest k > n such that n divides prime(k) - prime(n)
 * Range:     u8 n <= 22, u16 n <= 4276, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t p_n = get_prime(n);

    for (idx_t k = n + 1; ; ++k)
    {
        seq_t p_k = get_prime(k);
        if ((p_k - p_n) % (seq_t)n == 0)
            return (seq_t)k;
    }
}
