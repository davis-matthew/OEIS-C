/*
 * A082293  Numbers having exactly one square divisor > 1.
 * https://oeis.org/A082293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9968 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = nth integer that has exactly one square divisor > 1 */

#include <stdint.h>
#include <stdbool.h>

/* Helper: test whether an unsigned 64‑bit integer has exactly one square divisor > 1.
 * The function allows prime exponents 2 or 3. Any exponent ≥ 4 or more than one
 * prime with exponent ≥ 2 causes rejection. */
static bool has_one_square_divisor_u64(uint64_t n)
{
    int squares = 0;

    /* factor 2 separately for speed */
    if ((n & 1ULL) == 0) {
        int e = 0;
        while ((n & 1ULL) == 0) {
            n >>= 1;
            ++e;
            if (e > 3)            /* exponent 4 or more => >1 square divisor */
                return false;
        }
        if (e >= 2) {
            ++squares;
            if (squares > 1)
                return false;
        }
    }

    /* trial division by odd d up to sqrt(n) */
    for (uint64_t d = 3; d * d <= n; d += 2) {
        if (n % d == 0) {
            int e = 0;
            while (n % d == 0) {
                n /= d;
                ++e;
                if (e > 3)            /* exponent 4 or more => >1 square divisor */
                    return false;
            }
            if (e >= 2) {
                ++squares;
                if (squares > 1)
                    return false;
            }
        }
    }

    /* remaining factor n > 1 is a prime with exponent 1 -> no effect */
    return squares == 1;
}

/* Main sequence function.  For any order of calls, we keep an internal
 * counter only to find the nth term on the fly; no caching is used,
 * which keeps the code portable to all build types (including big‑int). */
seq_t a(idx_t n)
{
    /* n>=1 guarantees that the loop will hit the first term 4. */
    uint64_t count = 0;
    uint64_t cand   = 4;          /* first possible integer with a square divisor >1 */

    while (1) {
        if (has_one_square_divisor_u64(cand)) {
            ++count;
            if (count == n)
                return (seq_t)cand;
        }
        ++cand;
    }
}
