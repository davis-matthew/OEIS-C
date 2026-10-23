/*
 * A109921  a(2n) = prime(n). a(2n+1) = sum of composite numbers between
 *          prime(n) and prime(n+1). We define a(1) = 1.
 * https://oeis.org/A109921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 858, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:
 *   a(1) = 1
 *   a(2n)   = prime(n)
 *   a(2n+1) = sum of composite numbers between prime(n) and prime(n+1)
 *   (i.e. numbers x with prime(n) < x < prime(n+1) that are not prime)
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"   /* seq_t get_prime(uint64_t idx); */
#include "../utils/is_prime.h"    /* bool is_prime(uint64_t n); */

seq_t a(idx_t n)
{
    /* handle the special first term */
    if (n == 1)
        return (seq_t)1;

    /* even indices: a(2n) = prime(n) */
    if ((n & 1) == 0) {
        idx_t m = n >> 1;          /* n/2 */
        return get_prime(m);
    }

    /* odd indices: a(2n+1) = sum of composites between prime(n) and prime(n+1) */
    idx_t m = (n - 1) >> 1;        /* (n-1)/2 */
    seq_t p     = get_prime(m);    /* prime(n)   */
    seq_t p_next = get_prime(m + 1);/* prime(n+1) */

    /* Convert to uint64_t for the loop bounds (values are small for the tested range) */
    uint64_t pu     = (uint64_t)p;
    uint64_t pu_next = (uint64_t)p_next;

    seq_t sum = 0;
    for (uint64_t x = pu + 1; x < pu_next; ++x) {
        if (!is_prime(x))
            sum += (seq_t)x;
    }
    return sum;
}
