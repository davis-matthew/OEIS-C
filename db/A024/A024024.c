/*
 * A024024  a(n) = 3^n - n.
 * https://oeis.org/A024024
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n - n * This implementation uses binary exponentiation
 *            to compute 3^n * without intermediate overflow for the 64-bit
 *            variant up to n=40. * For larger n the result may wrap, but such
 *            values are not * required by the 64‑bit build. For arbitrary
 *            precision builds, * seq_t supports the usual...
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: compute 3^exp as seq_t using binary exponentiation */
static seq_t pow3(idx_t exp)
{
    seq_t result = 1;
    seq_t base   = 3;
    while (exp > 0) {
        if (exp & 1) {
            result = result * base;
        }
        exp >>= 1;
        if (exp)          /* avoid unnecessary overflow on the last iteration */
            base = base * base;
    }
    return result;
}

/* Main function for OEIS sequence A024024: a(n) = 3^n - n */
seq_t a(idx_t n)
{
    /* Special case n==0 to avoid unnecessary loop */
    if (n == 0) {
        return 1;                /* 3^0 - 0 = 1 */
    }
    seq_t p = pow3(n);
    return p - (seq_t)n;
}
