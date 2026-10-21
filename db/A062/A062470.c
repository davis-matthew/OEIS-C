/*
 * A062470  Nearest integer to log(n^n)^(n^(-1/n)).
 * https://oeis.org/A062470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nearest integer to (log(n^n))^(n^(-1/n))
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses long double arithmetic and rounds to the nearest integer. */
#include <stdint.h>
#include <math.h>

/* External linkage as required */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* not used, but guard */
    /* Use long double for better precision */
    long double nn = (long double)n;
    long double ln_n = log(nn);            /* natural logarithm of n */
    long double base = nn * ln_n;          /* log(n^n) = n * log(n) */
    long double exp = pow(nn, -1.0L / nn); /* n^(-1/n) */
    long double val = pow(base, exp);      /* (n*log n)^(n^(-1/n)) */
    /* Round to nearest integer; ties go to even as per llround */
    long long rounded = llround(val);
    return (seq_t)rounded;
}
