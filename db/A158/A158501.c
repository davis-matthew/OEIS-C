/*
 * A158501  Hankel transform of A158500.
 * https://oeis.org/A158501
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (n+1)*(2*(-1)^n*n^2 + 4*(-1)^n*n + 3*n + 3)/3
 * Range:     i8 n <= 5, i16 n <= 35, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n) {
    uint64_t un = (uint64_t)n;
    if ((un & 1ULL) == 0) {          /* even n */
        uint64_t num = 2ULL*un*un*un + 9ULL*un*un + 10ULL*un + 3ULL;
        return (seq_t)(num / 3ULL);
    } else {                        /* odd n */
        uint64_t num = 2ULL*un*un*un + 3ULL*un*un - 2ULL*un - 3ULL;
        return -(seq_t)(num / 3ULL);
    }
}
