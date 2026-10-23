/*
 * A048448  a(n) = prime(n-1) + prime(n+1) (assuming prime(i) = 0 for i < 1).
 * https://oeis.org/A048448
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n-1) + prime(n+1) (prime(i)=0 if i<1)
 * Range:     u8 n <= 31, u16 n <= 3513, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    seq_t v1 = 0, v2 = 0;
    if (n >= 1)
        v1 = (seq_t)get_prime(n - 1);
    if (n + 1 >= 1)
        v2 = (seq_t)get_prime(n + 1);
    return v1 + v2;
}
