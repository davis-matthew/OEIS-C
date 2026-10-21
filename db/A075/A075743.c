/*
 * A075743  For all numbers of the form 6*k +- 1 starting with 5, '1'
 *          indicates prime and '0' indicates composite.
 * https://oeis.org/A075743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 if 6*ceil(n/2)+(-1)^n is prime, otherwise 0
 * Range:     u8 n <= 8762, u16 n <= 8762, u32 n <= 8762, u64 n <= 8762,
 *            u128 n <= 8762, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* simple deterministic primality test suitable for numbers < 10^9
   (all values of 6*ceil(n/2)+(-1)^n for n≤10000 are < 60001) */
static inline bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    uint64_t i = 3;
    while (i * i <= n) {
        if (n % i == 0) return false;
        i += 2;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* compute the number k = 6*ceil(n/2)+(-1)^n */
    uint64_t half = (n + 1) / 2;          /* ceil(n/2) for unsigned n   */
    uint64_t k;
    if (n & 1ULL)          /* n is odd  -> (-1) term */
        k = 6 * half - 1;
    else                   /* n is even -> (+1) term */
        k = 6 * half + 1;

    return (seq_t)(is_prime_u64(k) ? 1 : 0);
}
