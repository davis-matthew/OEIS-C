/*
 * A212167  Numbers k such that the maximum exponent in its prime
 *          factorization is not greater than the number of positive exponents
 *          (A051903(k) <= A001221(k)).
 * https://oeis.org/A212167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth integer k such that the maximum exponent in its prime
 *            factorization is <= the number of distinct prime factors
 * Range:     u8 n <= 210, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a212167_valid_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/a212167_valid_uint64.h"

/* Main sequence function (external linkage). */
seq_t a(idx_t n)
{
    /* n is 1‑based offset. We simply search natural numbers until
       we have found n valid numbers. */
    uint64_t found = 0;
    uint64_t candidate = 1;

    while (found < n) {
        if (a212167_valid_uint64(candidate))
            ++found;
        ++candidate;
    }

    /* candidate has been incremented one past the desired number */
    return (seq_t)(candidate - 1);
}
