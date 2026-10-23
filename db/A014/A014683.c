/*
 * A014683  In the sequence of positive integers add 1 to each prime number.
 * https://oeis.org/A014683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + (isPrime(n) ? 1 : 0)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = n + pi(n) - pi(n-1)  since pi(n)-pi(n-1) is 1 iff n is prime   */

#include <stdbool.h>
#include <stdint.h>

/* idx_t and seq_t are supplied by the build environment. */

/* Simple deterministic primality test for idx_t values.
   Works efficiently for the range of n that occurs in the tests
   (up to about 10 000 in the problem description). */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t i = 3; i <= n / i; i += 2) {
        if (n % i == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* outside the OEIS offset – defensive */
    seq_t val = (seq_t)n;                /* promote index to sequence type */
    if (is_prime_uint64(n))
        val += (seq_t)1;                 /* add 1 when n is prime   */
    return val;
}
