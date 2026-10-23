/*
 * A014229  Inverse of 220th cyclotomic polynomial.
 * https://oeis.org/A014229
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Inverse of the 220th cyclotomic polynomial.
   The coefficients a(n) form a periodic sequence of period 220.
   Computed via the product representation
        1/Φ_220(x) = Π_{d|220} (1 - x^d)^{-μ(220/d)},
   where μ is the Möbius function.
   This generates the integer coefficients a(n) exactly. */
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t seq[220];
    if (!initialized) {
        /* a[0] = 1, other entries start at 0 automatically. */
        seq[0] = 1;
        /* List of divisor exponent signs for 1/Φ_220(x). */
        const idx_t  ds[8]   = {2, 4, 10, 20, 22, 44, 110, 220};
        const int    signs[8] = {-1, 1, 1, -1, 1, -1, -1, 1};

        for (int k = 0; k < 8; ++k) {
            idx_t d = ds[k];
            if (signs[k] > 0) {
                /* Multiply by (1 - x^d)^(-1) : add shifted copies. */
                for (idx_t i = d; i < 220; ++i) {
                    seq[i] += seq[i - d];
                }
            } else {
                /* Multiply by (1 - x^d) : subtract shifted copies. */
                for (idx_t i = 219; i >= d; --i) {
                    seq[i] -= seq[i - d];
                }
            }
        }
        initialized = true;
    }

    idx_t idx = n % 220;
    return seq[idx];
}
