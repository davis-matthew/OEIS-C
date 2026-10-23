/*
 * A014534  Inverse of 525th cyclotomic polynomial.
 * https://oeis.org/A014534
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n in 1/Φ_525(x), periodic with period
 *            525
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t cache[525];
static bool computed = false;

static void compute_cache(void)
{
    const idx_t LEN = 525;
    seq_t a[525];
    for (idx_t i = 0; i < LEN; ++i) a[i] = 0;
    a[0] = 1;

    /* Multiply by (1 - x^d) for d in {5, 75, 105, 175} */
    const idx_t Splus[] = {5, 75, 105, 175};
    const idx_t np     = 4;
    for (idx_t pi = 0; pi < np; ++pi) {
        idx_t d = Splus[pi];
        seq_t tmp[525];
        for (idx_t i = 0; i < LEN; ++i) tmp[i] = a[i];
        for (idx_t i = d; i < LEN; ++i) tmp[i] -= a[i - d];
        for (idx_t i = 0; i < LEN; ++i) a[i] = tmp[i];
    }

    /* Multiply by 1/(1 - x^d) for d in {15, 25, 35, 525} */
    const idx_t Sminus[] = {15, 25, 35, 525};
    const idx_t nm      = 4;
    for (idx_t mi = 0; mi < nm; ++mi) {
        idx_t d = Sminus[mi];
        for (idx_t i = d; i < LEN; ++i) a[i] += a[i - d];
    }

    for (idx_t i = 0; i < LEN; ++i) cache[i] = a[i];
    computed = true;
}

seq_t a(idx_t n)
{
    if (!computed) compute_cache();
    return cache[(idx_t)(n % 525)];
}
