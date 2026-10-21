/*
 * A067666  Sum of squares of prime factors of n (counted with multiplicity).
 * https://oeis.org/A067666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ p_i^2, where the p_i are the prime factors of n counted
 *            with multiplicity. a(1) = 0. For n >= 2: factor n by trial
 *            division and accumulate square of each prime factor
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* For the OEIS offset a(1) = 0.  Index 0 is out of range. */
    if (n <= 1)
        return (seq_t)0;

    /* Use a local variable of native integer type for factorisation. */
    uint64_t m = (uint64_t)n;
    seq_t sum = (seq_t)0;

    /* Trial division by 2 first. */
    while ((m & 1ULL) == 0) {
        sum += (seq_t)2 * (seq_t)2;
        m >>= 1;
    }

    /* Odd primes. p*p <= m is sufficient for trial division. */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        while (m % p == 0) {
            sum += (seq_t)p * (seq_t)p;
            m /= p;
        }
    }

    /* If m > 1 now, it is a prime factor larger than sqrt(original n). */
    if (m > 1)
        sum += (seq_t)m * (seq_t)m;

    return sum;
}
