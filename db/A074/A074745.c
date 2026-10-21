/*
 * A074745  a(n) = sum_{k=1..n} prime(k)*prime(k+1).
 * https://oeis.org/A074745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1..n} prime(k) * prime(k+1)
 * Range:     u8 n <= 4, u16 n <= 24, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"  /* declares uint32_t get_prime(idx_t k) */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t pk  = (seq_t)get_prime(k);
        seq_t pk1 = (seq_t)get_prime(k + 1);
        sum += pk * pk1;
    }
    return sum;
}
