/*
 * A163407  Sum of semiprime divisors of n with repetition.
 * https://oeis.org/A163407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (s² - ss)/2 where s = sum of prime divisors of n with
 *            multiplicity, ss = sum of squares of those primes with the same
 *            multiplicity
 * Range:     u8 n <= 258, u16 n <= 9730, u32 n <= 9730, u64 n <= 9730,
 *            u128 n <= 9730, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Function to compute A163407 for n >= 1. */
seq_t a(idx_t n) {
    if (n <= 1)
        return (seq_t)0;

    /* Use 128‑bit integers for intermediate arithmetic to avoid overflow. */
    unsigned __int128 s128 = 0;
    unsigned __int128 ss128 = 0;

    uint64_t m = (uint64_t)n;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            unsigned int e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            unsigned __int128 sp = (unsigned __int128)p;
            s128  += sp * (unsigned __int128)e;
            ss128 += sp * sp * (unsigned __int128)e;
        }
    }
    /* Remaining prime factor > sqrt(n). */
    if (m > 1) {
        unsigned __int128 sp = (unsigned __int128)m;
        s128  += sp;
        ss128 += sp * sp;
    }

    unsigned __int128 result128 = (s128 * s128 - ss128) / 2;
    return (seq_t)result128;
}
