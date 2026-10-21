/*
 * A090370  Least m > 3 such that gcd(n-1, m*n - 1) = m-1.
 * https://oeis.org/A090370
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + smallest divisor > 2 of (n-1)
 * Range:     u8 n <= 257, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Ensure we work with unsigned 64‑bit arithmetic.  */
    uint64_t N = n - 1;              /* N = n-1  (valid for n ≥ 4) */
    /* Scan for the smallest divisor d > 2 of N.  */
    for (uint64_t d = 3; ; ++d) {
        if (N % d == 0) {
            return (seq_t)(d + 1);   /* a(n) = d + 1  */
        }
    }
}
