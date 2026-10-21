/*
 * A093024  Indices of terms in A093019 with value 4.
 * https://oeis.org/A093024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 6554, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   required_luhn_check_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = the n‑th positive integer m whose required
 * Luhn check digit equals 4.  The Luhn check digit for a prefix
 * m is computed by doubling the rightmost digit of m, then
 * alternating doubling and not, and finally determining d such
 * that (sum + d) % 10 == 0. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/required_luhn_check_digit.h"

/* External function required by the build. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* undefined for n < 1, return 0 as safety */

    idx_t count   = 0;
    idx_t candidate = 1;

    for (;;) {
        if (required_luhn_check_digit(candidate) == 4) {
            ++count;
            if (count == n)
                return (seq_t)candidate;     /* cast to seq_t; truncation OK when out of range */
        }
        ++candidate;
    }
}
