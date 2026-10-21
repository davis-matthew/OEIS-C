/*
 * A081303  a(n) = gpf(n) - 2*lpf(n), where gpf(n) is the greatest and lpf(n)
 *          is the least prime factor of n (A006530, A020639).
 * https://oeis.org/A081303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = gpf(n) - 2*lpf(n) (OEIS A081303)
 * Range:     i8 n <= 130, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special case for n = 1 */
    if (n == 1) {
        return (seq_t)-1;
    }

    /* Work with 64‑bit unsigned integers for factorisation */
    uint64_t x = (uint64_t)n;
    uint64_t lpf_val = 0;   /* least prime factor */
    uint64_t gpf_val = 0;   /* greatest prime factor */
    uint64_t rem = x;

    /* Handle factor 2 separately */
    if ((rem & 1ULL) == 0) {
        lpf_val = 2ULL;
        gpf_val = 2ULL;
        while ((rem & 1ULL) == 0) {
            rem >>= 1;
        }
    }

    /* Test odd divisors up to sqrt(rem) */
    for (uint64_t d = 3ULL; d * d <= rem; d += 2ULL) {
        if (rem % d == 0) {
            if (lpf_val == 0) {
                lpf_val = d;
            }
            gpf_val = d;
            while (rem % d == 0) {
                rem /= d;
            }
        }
    }

    /* If rem > 1, it is a prime greater than sqrt(x) */
    if (rem > 1ULL) {
        if (lpf_val == 0) {
            lpf_val = rem;
        }
        gpf_val = rem;
    }

    /* Compute the value in a wider signed type to avoid overflow on small
       seq_t implementations, then cast back to seq_t. */
    int64_t diff = (int64_t)gpf_val - (int64_t)(2ULL * lpf_val);
    return (seq_t)diff;
}
