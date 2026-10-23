/*
 * A014971  Inverse of 962nd cyclotomic polynomial.
 * https://oeis.org/A014971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n in * (1 - x)(1 - x^26)(1 - x^74)(1 -
 *            x^481) * ÷ (1 - x^2)(1 - x^13)(1 - x^37) * This sequence is
 *            periodic with period 962
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

/* periodic table of length 962 */
static seq_t periodic[962];
static int periodic_filled = 0;

/* compute the period once */
static void build_period(void)
{
    if (periodic_filled) return;
    const size_t M = 962;

    /* start with 1/(1 - x^2)(1 - x^13)(1 - x^37) */
    for (size_t i = 0; i < M; ++i) periodic[i] = 0;
    periodic[0] = 1;

    /* multiply by 1/(1 - x^d) -> forward recurrence */
    const size_t series_ds[] = { 2, 13, 37 };
    for (size_t di = 0; di < sizeof(series_ds)/sizeof(series_ds[0]); ++di) {
        size_t d = series_ds[di];
        for (size_t i = d; i < M; ++i) {
            periodic[i] += periodic[i - d];
        }
    }

    /* multiply by (1 - x^s) -> backward subtraction */
    const size_t poly_ss[] = { 26, 74, 481, 1 };
    for (size_t si = 0; si < sizeof(poly_ss)/sizeof(poly_ss[0]); ++si) {
        size_t s = poly_ss[si];
        for (size_t i = M - 1; i + 1 > s; --i) {   /* i descending */
            periodic[i] -= periodic[i - s];
        }
    }

    periodic_filled = 1;
}

seq_t a(idx_t n)
{
    build_period();
    const size_t M = 962;
    size_t idx = (size_t)(n % M);
    return periodic[idx];
}
