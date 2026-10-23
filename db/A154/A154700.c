/*
 * A154700  Prime divisors of 2^36-1.
 * https://oeis.org/A154700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Prime divisors of 2^36-1 (finite list). */
#include <stdint.h>
seq_t a(idx_t n)
{
    /* cache the prime divisors once */
    static seq_t primes[10];
    static idx_t prime_count = 0u;
    if (prime_count == 0) {
        /* 2^36 - 1 */
        uint64_t N = 68719476735ull;
        uint64_t tmp = N;
        for (uint64_t p = 2; p * p <= tmp; ++p) {
            if (tmp % p == 0) {
                primes[prime_count++] = (seq_t)p;
                while (tmp % p == 0)
                    tmp /= p;
            }
        }
        if (tmp > 1)            /* remaining prime factor */
            primes[prime_count++] = (seq_t)tmp;
        /* primes are stored in ascending order */
    }
    if (n == 0 || n > prime_count)
        return (seq_t)0;
    return primes[n - 1];
}
