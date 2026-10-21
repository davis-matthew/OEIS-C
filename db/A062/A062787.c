/*
 * A062787  a(n) = floor(exp(gamma) n log log n ) - phi(n), where gamma is
 *          Euler's constant (A001620).
 * https://oeis.org/A062787
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = floor(exp(gamma) * n * log log n) - phi(n)
 * Range:     i8 n <= 59, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   phi_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/phi_uint64.h"

/* Computes the nth term of sequence A062787 */
seq_t a(idx_t n)
{
    /* Values below the OEIS offset are undefined; return 0 for safety */
    if (n < 2) return 0;

    /* Natural logarithm of n and of that value */
    double ln  = log((double)n);
    double lln = log(ln);

    /* exp(gamma) ≈ 1.781072417990197 */
    static const double e_gamma = 1.781072417990197;

    /* Compute the real part of the formula */
    double val = e_gamma * (double)n * lln;

    /* Convert the double result to an integer (floor) safely */
    long long fl = (long long)floor(val);   /* value fits in 64‑bit for n up to ~10^12 */
    seq_t floor_term = (seq_t)fl;

    /* Euler's totient function, using the helper in utils */
    seq_t phi_val = (seq_t)phi_uint64((uint64_t)n);

    /* Final result */
    return floor_term - phi_val;
}
