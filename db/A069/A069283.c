/*
 * A069283  a(n) = -1 + number of odd divisors of n.
 * https://oeis.org/A069283
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (# of odd divisors of n) - 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes a(n) for OEIS sequence A069283.
   For n = 0 returns 0 (by definition).
   For n > 0, returns the number of odd divisors of n minus 1. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Extract the odd part of n */
    uint64_t m = n;
    while ((m & 1ULL) == 0)
        m >>= 1;

    seq_t count = 1;         /* number of odd divisors */
    uint64_t p = 3;

    /* Factor the odd part by trial division by odd primes */
    while (p <= m / p) {
        if (m % p == 0) {
            uint64_t exp = 0;
            while (m % p == 0) {
                m /= p;
                exp++;
            }
            count *= (seq_t)(exp + 1);
        }
        p += 2;
    }

    /* If any prime factor > sqrt remains, account for it */
    if (m > 1)
        count *= 2;

    return count - 1;
}
