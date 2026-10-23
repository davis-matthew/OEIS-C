/*
 * A152530  a(n) = p(n)*p(n+2)-p(n+1), where p(n) is the n-th prime.
 * https://oeis.org/A152530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = p(n)*p(n+2)-p(n+1)
 * Range:     u8 n <= 6, u16 n <= 53, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* offset is 1; undefined behaviour for n==0 */
    uint32_t p1 = get_prime(n);
    uint32_t p2 = get_prime(n + 1);
    uint32_t p3 = get_prime(n + 2);

    seq_t val = (seq_t)p1 * (seq_t)p3 - (seq_t)p2;
    return val;
}
