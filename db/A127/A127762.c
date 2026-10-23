/*
 * A127762  Integer part of Gauss's Arithmetic-Geometric Mean M(2,n^2).
 * https://oeis.org/A127762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( AGM(2, n^2) )
 * Range:     u8 none, u16 none, u32 none, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   floor_sqrt, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the integer part of Gauss's Arithmetic‑Geometric‑Mean
   M(2, n²) using fixed‑point arithmetic (scale 10⁶). */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_mul.h"     /* seq_mul() – exact integer multiplication */
#include "../utils/floor_sqrt.h"  /* floor_sqrt() – integer square root */

seq_t a(idx_t n)
{
    if (n == 0)         /* offset 1 – harmless for n = 0 */
        return 0;

    /* scaling factor 10⁶ (sufficient for n ≤ 10⁴ and all tested terms) */
    const seq_t SCALE = (seq_t)1000000U;

    /* start values for the AGM iteration:
       a₀ = 2,   b₀ = n²   (all multiplied by SCALE) */
    seq_t a_val = (seq_t)2 * SCALE;
    seq_t b_val = (seq_t)n * (seq_t)n * SCALE;

    /* maintain a_val ≤ b_val */
    if (a_val > b_val) {
        seq_t tmp = a_val;
        a_val = b_val;
        b_val = tmp;
    }

    /* iterate until successive values differ by at most one in the
       fixed‑point representation (i.e. < 1/SCALE in real terms). */
    while (b_val > a_val + 1U) {
        /* arithmetic mean */
        seq_t new_a = (a_val + b_val) / 2U;

        /* geometric mean: floor(sqrt(a*b))  (both are scaled) */
        seq_t prod   = seq_mul(a_val, b_val);
        seq_t new_b  = floor_sqrt(prod);

        /* preserve ordering: a_val ≤ b_val */
        if (new_a > new_b) {
            seq_t tmp = new_a;
            new_a = new_b;
            new_b = tmp;
        }

        a_val = new_a;
        b_val = new_b;
    }

    /* strip the scaling factor and return the integer part. */
    return a_val / SCALE;
}
