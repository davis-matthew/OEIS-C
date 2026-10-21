/*
 * A224877  Expansion of f(x^29, x^31) - x^3 * f(x^11, x^49) in powers of x
 *          where f() is a Ramanujan theta function.
 * https://oeis.org/A224877
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 1 if n = k(30k + 1) for some k∈ℤ, * a(n) = -1 if n =
 *            k(30k + 19) + 3 for some k∈ℤ, * a(n) = 0 otherwise. Equivalent
 *            condition: * Let D = 1 + 120n and m = √D (integer). * If m ≡ 1
 *            or 59 (mod 60) → a(n)=1, * if m ≡ 19 or 41 (mod 60) → a(n)=-1, *
 *            otherwise a(n)=0. Checked via integer arithmetic
 * Range:     i8 n <= 0, i16 n <= 85, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Compute D = 1 + 120*n */
    seq_t D = (seq_t)120 * (seq_t)n + (seq_t)1;

    /* Integer square root of D */
    seq_t m = floor_sqrt(D);

    /* Exact square test */
    if (m * m != D)
        return (seq_t)0;

    /* Remainder modulo 60 */
    seq_t r = m % (seq_t)60;

    if (r == (seq_t)1 || r == (seq_t)59)
        return (seq_t)1;
    if (r == (seq_t)19 || r == (seq_t)41)
        return (seq_t)-1;

    return (seq_t)0;
}
