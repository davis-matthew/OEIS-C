/*
 * A045661  a(n) = Product_{d|n} (n/d + d).
 * https://oeis.org/A045661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = Product_{d|n} (n/d + d)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 29, u64 n <= 119, u128 n <= 359,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

/* Computes a(n) = Product_{d|n} (n/d + d)
 * for n >= 1.  The result is returned in type seq_t,
 * which is defined by the build system (an unsigned
 * integer type of any size or an arbitrary-precision class).
 */
seq_t a(idx_t n) {
    if (n == 0) return 0;  /* offset starts at 1, but guard anyway */
    seq_t prod = 1;
    uint64_t num = (uint64_t)n;

    /* integer square root of num */
    uint64_t sq = (uint64_t)floor(sqrt((long double)num));
    while ((sq + 1) * (sq + 1) <= num) ++sq;
    while (sq * sq > num) --sq;

    for (uint64_t d = 1; d <= sq; ++d) {
        if (num % d == 0) {
            uint64_t d1 = d;
            uint64_t d2 = num / d;
            seq_t term = (seq_t)d1 + (seq_t)d2;
            if (d1 == d2) {
                prod *= term;           /* perfect square divisor */
            } else {
                prod *= term;           /* divisor d */
                prod *= term;           /* its pair n/d */
            }
        }
    }
    return prod;
}
