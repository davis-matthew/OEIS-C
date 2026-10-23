/*
 * A010235  Continued fraction for sqrt(193).
 * https://oeis.org/A010235
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction coefficients of sqrt(193).
   a(0) = 13, and for n ≥ 1 the terms repeat with period
   [1, 8, 3, 2, 1, 3, 3, 1, 2, 3, 8, 1, 26] */
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* One‑time initialization of the periodic part. */
    static bool initialized = false;
    static seq_t period[64];
    static size_t period_len = 0;

    const uint64_t D  = 193ULL;   /* Number under the square root. */
    const uint64_t a0 = 13ULL;    /* floor(sqrt(D)). */

    if (!initialized) {
        uint64_t m = 0;
        uint64_t d = 1;
        uint64_t a = a0;
        do {
            m = d * a - m;
            d = (D - m * m) / d;
            a = (a0 + m) / d;
            period[period_len++] = (seq_t)a;
        } while (a != 2 * a0);   /* Terminate when the last partial
                                   quotient is 2*a0. */
        initialized = true;
    }

    if (n == 0ULL) {
        return (seq_t)a0;
    }

    /* For n ≥ 1 return the (n‑1)-th element of the period. */
    size_t idx = (size_t)((n - 1ULL) % (idx_t)period_len);
    return period[idx];
}
