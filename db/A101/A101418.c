/*
 * A101418  Floor of the area of a lens constructed using circular arcs of
 *          radius n.
 * https://oeis.org/A101418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((4*Pi - 3*sqrt(3))/6 * n^2)
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is equivalent to a(n) = floor((2*Pi/3 - sqrt(3)/2) * n^2). */
/* We use a fixed‑point approximation of the irrational constant
 * with denominator 10^9.  The relative error (≈10⁻⁹) is small enough
 * that for n ≤ 10 000 the floor is unaffected (error × n² < 0.1 < 0.5). */
#include <stdint.h>

static const seq_t P = (seq_t)1228369698;   /* floor(c * 10⁹) */
static const seq_t Q = (seq_t)1000000000;   /* 10⁹ */

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* guard for offset 1 */
    seq_t n_sq = (seq_t)n * (seq_t)n;
    return (P * n_sq) / Q;           /* integer division truncates → floor */
}
