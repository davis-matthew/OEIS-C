/*
 * A050759  Numbers k such that the decimal expansion of (k^2 + k)/2 (the k-th
 *          triangular number) contains no pair of consecutive equal digits.
 * https://oeis.org/A050759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, changed
 * Formula:   not stated
 * Range:     u8 n <= 190, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   no_consecutive_equal_digits_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A050759:
 *  a(n) = the nth integer k such that the k‑th triangular number
 *  T_k = k*(k+1)/2 has no pair of consecutive equal decimal digits.
 */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/no_consecutive_equal_digits_uint64.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        /* Undefined for n = 0, but return 0 as a safe default. */
        return (seq_t)0;
    }

    idx_t found = 0;   /* number of valid k found so far */
    idx_t k = 0;       /* current candidate k */

    while (found < n) {
        /* Compute the k‑th triangular number using 64‑bit arithmetic. */
        uint64_t tri = (uint64_t)k * (k + 1) / 2;

        /* Test for consecutive equal digits. */
        if (no_consecutive_equal_digits_uint64(tri)) {
            ++found;
            if (found == n) {
                return (seq_t)k;
            }
        }

        ++k;
    }

    /* Should never reach here. */
    return (seq_t)0;
}
