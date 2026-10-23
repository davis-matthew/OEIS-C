/*
 * A188020  [nr]-[nr-kr]-[kr], where r=(1+sqrt(5))/2, k=7, [ ]=floor.
 * https://oeis.org/A188020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊n⋅φ⌋ – ⌊(n−7)⋅φ⌋ – ⌊7⋅φ⌋, where φ = (1+√5)/2. For n<7 we
 *            use a(n) = ⌊n⋅φ⌋ + ⌊(7−n)⋅φ⌋ – 10 (derived from the same
 *            formula)
 * Range:     u8 none, u16 n <= 57, u32 n <= 117, u64 n <= 117, u128 n <= 117,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

/* Compute ⌊n⋅φ⌋ for a non‑negative integer n (idx_t).  */
static inline seq_t floor_phi(idx_t n)
{
    seq_t M = (seq_t)n;           /* convert to seq_t */
    seq_t temp = M * M;           /* M² */
    seq_t t = floor_sqrt((seq_t)5 * temp);  /* ⌊√(5·M²)⌋ */
    seq_t sum = M + t;            /* M + ⌊√(5·M²)⌋ */
    return sum / 2;               /* integer division gives   ⌊(M+…)/2⌋ = ⌊M⋅φ⌋ */
}

seq_t a(idx_t n)
{
    /* floor(7⋅φ) = 11 */
    const seq_t const_7phi = 11;

    if (n >= 7) {
        seq_t fn = floor_phi(n);
        seq_t fm = floor_phi(n - 7);
        return fn - fm - const_7phi;
    } else {
        /* n < 7 : use a(n) = ⌊n⋅φ⌋ + ⌊(7−n)⋅φ⌋ – 10 */
        seq_t fn = floor_phi(n);
        seq_t fp = floor_phi(7 - n);
        return fn + fp - 10;
    }
}
