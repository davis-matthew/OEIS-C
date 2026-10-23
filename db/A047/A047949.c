/*
 * A047949  a(n) is the largest m such that n-m and n+m are both primes, or -1
 *          if no such m exists.
 * https://oeis.org/A047949
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   not stated
 * Range:     u8 n <= 261, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A047949: a(n) = max{ m | n-m and n+m are both primes }, -1 if none.
 * This implementation directly tests primes in decreasing order of m.
 * Guarantees correct results for any n ≥ 2. Uses the auxiliary
 * <../utils/is_prime.h> function for primality testing on idx_t values.
 */
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n < 2) {                 /* offset starts at 2, but guard for safety */
        return (seq_t)-1;
    }

    /* The maximum possible m is n-2 (since n-m must be ≥ 2). */
    for (idx_t m = n - 2; ; --m) {
        idx_t p = n - m;
        idx_t q = n + m;

        if (is_prime(p) && is_prime(q)) {
            return (seq_t)m;     /* found the largest m */
        }

        if (m == 0) {
            break;                /* no further values possible */
        }
    }

    return (seq_t)-1;            /* no such m exists */
}
