/*
 * A038605  a(n) = floor( prime(n)/n ).
 * https://oeis.org/A038605
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(prime(n)/n)
 * Range:     u8 n <= 54, u16 n <= 7002, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* For indices < 1 the behaviour is undefined; return 0 in this case */
    if (n == 0) return 0;
    return ((seq_t)get_prime(n)) / (seq_t)n;
}
