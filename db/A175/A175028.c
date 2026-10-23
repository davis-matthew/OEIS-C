/*
 * A175028  a(n)=n+1 if n is prime, otherwise a(n)=n-1.
 * https://oeis.org/A175028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + 1 if n is prime, else a(n) = n - 1
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    return is_prime(n) ? (seq_t)n + 1 : (seq_t)n - 1;
}
