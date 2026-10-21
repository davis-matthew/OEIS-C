/*
 * A222010  Dimensions of spheres that admit continuous multiplications with
 *          unit element.
 * https://oeis.org/A222010
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, nice
 * Formula:   a(n) = 2^n - 1 (for all n ≥ 0)
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (This follows from a(n) = A222011(n) - 1 with A222011(n) = 2^n.) */
#include "../utils/pow2.h"   /* provides seq_t pow2(idx_t exp) */

seq_t a(idx_t n)
{
    /* compute 2^n - 1 in seq_t type, works for any seq_t width */
    seq_t p = pow2(n);   /* pow2(0) = 1 */
    return p - 1;
}
