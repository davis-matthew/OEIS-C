/*
 * A018115  Powers of fourth root of 24 rounded to nearest integer.
 * https://oeis.org/A018115
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(24^(n/4))
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   floor_sqrt, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * For small integral types (8, 16, 32 or 64 bit) the value
 * safely fits in a long double and can be rounded directly.
 *
 * For arbitrary‑precision types the value may be enormous; we
 * compute 24^n with big integer arithmetic and then take its
 * fourth root exactly.
 *
 * No floating point arithmetic is used on the final integer value.
 */
#include <math.h>       /* for powl, llrintl */
#include "../utils/seq_pow.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* ---- small integral types --------------------------------------------------- */
    if (sizeof(seq_t) <= sizeof(unsigned long long))
    {
        /* Compute with long double and round to nearest integer. */
        long double exponent = (long double)n / 4.0L;
        long double value   = powl((long double)24.0L, exponent);
        return (seq_t)llrintl(value);   /* round to nearest integer */
    }

    /* ---- arbitrary‑precision types --------------------------------------------- */
    /* Pre‑compute 24^n */
    seq_t pow24 = seq_pow((seq_t)24, n);

    /* Four‑th root by successive integer square roots */
    seq_t sqrt1 = floor_sqrt(pow24);
    seq_t sqrt2 = floor_sqrt(sqrt1);
    seq_t low   = sqrt2;             /* floor(24^(n/4)) */

    /* Compute low^4 and (low+1)^4 for comparison */
    seq_t lowSq    = seq_pow(low,    2);
    seq_t low4     = seq_pow(lowSq,  2);

    seq_t high   = low + 1;          /* ceil(24^(n/4)) */
    seq_t highSq  = seq_pow(high,   2);
    seq_t high4   = seq_pow(highSq, 2);

    /* Choose the nearer integer */
    seq_t diffLow  = pow24 - low4;
    seq_t diffHigh = high4 - pow24;

    if (diffLow <= diffHigh)
        return low;
    return high;
}
