/*
 * A049419  a(1) = 1; for n > 1, a(n) = number of exponential divisors of n.
 * https://oeis.org/A049419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, nice, easy
 * Formula:   a(n) = number of exponential divisors of n. Multiplicative with
 *            a(p^e) = tau(e), tau(m) = number of divisors of m
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   tau_of_exp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/tau_of_exp.h"

seq_t a(idx_t n)
{
    if (n == 1) return 1;   /* by definition */

    uint64_t x = (uint64_t)n;
    seq_t result = 1;
    int cnt = 0;

    /* factor 2 */
    while ((x & 1ULL) == 0) {
        x >>= 1;
        cnt++;
    }
    if (cnt > 0) {
        result *= tau_of_exp(cnt);
    }

    /* odd factors */
    for (uint64_t p = 3; p * p <= x; p += 2) {
        cnt = 0;
        while (x % p == 0) {
            x /= p;
            cnt++;
        }
        if (cnt > 0) {
            result *= tau_of_exp(cnt);
        }
    }

    /* if a prime remains */
    if (x > 1) {
        /* remaining prime has exponent 1, tau(1) == 1 */
        result *= tau_of_exp(1);
    }

    return result;
}
