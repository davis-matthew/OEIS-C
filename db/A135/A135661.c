/*
 * A135661  a(n) = floor(sqrt(n) - n^(1/3)).
 * https://oeis.org/A135661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( sqrt(n) - n^(1/3) ) (offset 1)
 * Range:     u8 n <= 9, u16 n <= 3, u32 n <= 3, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   floor_cube_root, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <assert.h>
#include "../utils/floor_sqrt.h"
#include "../utils/floor_cube_root.h"

/* A moderate scaling factor that keeps intermediate products within
   64‑bit limits for n up to 10 000. */
static const seq_t SCALE = 10000;         /* 1e4 */
static const seq_t SCALE2 = SCALE * SCALE;      /* 1e8  */
static const seq_t SCALE3 = SCALE2 * SCALE;     /* 1e12 */

/* Compute a(n).  idx_t n is at least 1. */
seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;

    /* Integer square and cube roots. */
    seq_t s = floor_sqrt(n_seq);          /* s = floor(sqrt(n))   */
    seq_t t = floor_cube_root(n_seq);     /* t = floor(cuberoot(n)) */

    /* d = s - t  (always non‑negative for n ≥ 1). */
    seq_t d = s >= t ? s - t : (seq_t)0;   /* safety for n=0 (unused) */

    if (d == 0)
        return (seq_t)0;                  /* floor diff is 0 */

    /* Scale n to get finer resolution of the fractional parts. */
    seq_t sqrt_scaled   = floor_sqrt(n_seq * SCALE2);   /* floor(sqrt(n)*SCALE)   */
    seq_t cbrt_scaled   = floor_cube_root(n_seq * SCALE3); /* floor(cbrt(n)*SCALE) */

    /* Fractional parts multiplied by SCALE. */
    seq_t frac_sqrt = sqrt_scaled - (s * SCALE);
    seq_t frac_cbrt = cbrt_scaled - (t * SCALE);

    /* Decide whether α ≥ β. */
    if (frac_sqrt >= frac_cbrt)
        return d;          /* floor(sqrt(n) - cbrt(n)) = d */
    else
        return d - 1;      /* floor(sqrt(n) - cbrt(n)) = d‑1 */
}
