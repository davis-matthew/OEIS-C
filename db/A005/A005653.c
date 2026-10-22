/*
 * A005653  Lexicographically least increasing sequence, starting with 2, such
 *          that the sum of two distinct terms of the sequence is never a
 *          Fibonacci number.
 * https://oeis.org/A005653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Positive integers k where {k*phi} < 1/2, with phi =
 *            (1+sqrt(5))/2
 * Range:     u8 n <= 127, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>


seq_t a(idx_t n) {
    const double phi = (1.0 + sqrt(5.0)) / 2.0;
    
    idx_t count = 0;
    for (idx_t k = 1; ; k++) {
        double product = (double)k * phi;
        double frac = product - floor(product);
        if (frac < 0.5) {
            count++;
            if (count == n) {
                return (seq_t)k;
            }
        }
    }
}
