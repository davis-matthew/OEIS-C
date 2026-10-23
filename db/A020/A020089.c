/*
 * A020089  Integer part of Gamma(n + 1/3)/Gamma(1/3).
 * https://oeis.org/A020089
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊Γ(n + 1/3) / Γ(1/3)⌋ = ⌊∏_{k=0}^{n‑1} (3k + 1) / 3^n⌋
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* For integral types that fit in 64 bits we use a larger
       intermediate type to avoid overflow while computing
       the numerator and denominator.  For arbitrary‑precision
       types the built‑in seq_t can hold the intermediate value. */
    if (sizeof(seq_t) <= 8) {
        __int128 num = 1;
        __int128 den = 1;
        for (idx_t k = 0; k < n; ++k) {
            num *= (3 * k + 1);
            den *= 3;
        }
        return (seq_t)(num / den);
    } else {
        seq_t num = 1;
        seq_t den = 1;
        for (idx_t k = 0; k < n; ++k) {
            num *= (seq_t)(3 * k + 1);
            den *= (seq_t)3;
        }
        return num / den;
    }
}
