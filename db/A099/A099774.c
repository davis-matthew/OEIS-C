/*
 * A099774  Number of divisors of 2*n-1.
 * https://oeis.org/A099774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of divisors of 2*n-1 (τ(2*n-1))
 * Range:     u8 n <= 2189, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 9694 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Compute the divisor count of an odd number m.
   m is expected to fit in uint64_t. */
static seq_t divisor_count_odd(uint64_t m)
{
    if (m == 1) {
        return 1;          /* 1 has exactly one divisor */
    }

    seq_t tau = 1;
    uint64_t tmp = m;
    uint64_t factor = 3;       /* 2 is not a divisor of an odd number */
    while (((__int128)factor * factor) <= tmp) {
        uint64_t exp = 0;
        while (tmp % factor == 0) {
            tmp /= factor;
            exp++;
        }
        if (exp) {
            tau *= (exp + 1);
        }
        factor += 2;
    }
    if (tmp > 1) {             /* remaining prime factor */
        tau *= 2;
    }
    return tau;
}

seq_t a(idx_t n)
{
    /* 2*n-1 is guaranteed odd */
    uint64_t m = (uint64_t)(2 * (seq_t)n - 1);
    return divisor_count_odd(m);
}
