/*
 * A016042  Inverse of 2033rd cyclotomic polynomial.
 * https://oeis.org/A016042
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 3000, i16 n <= 3000, i32 n <= 3000, i64 n <= 3000,
 *            i128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A016042:  Inverse of the 2033rd cyclotomic polynomial.
 *  The coefficients of 1/Φ_2033(x) form a periodic sequence of
 *  period 2033 that can be described explicitly:
 *
 *  a(n) =  1   , if (n mod 2033) ∈ [0,18]
 *  a(n) =  0   , if (n mod 2033) ∈ [19,106] ∪ [126,2032]
 *  a(n) = -1   , if (n mod 2033) ∈ [107,125]
 *
 *  This follows from the factorisation
 *      Φ_2033(x)·(x−1)·Φ_19(x)·Φ_107(x) = x^2033−1
 *  and the fact that 1/Φ_2033(x) = −(x−1)·Φ_19(x)·Φ_107(x)/(x^2033−1).
 */
#include <stdint.h>

seq_t a(idx_t n) {
    /* Reduce the index modulo 2033 (period of the sequence).  */
    idx_t r = n % 2033ULL;

    /* Return the appropriate value according to the explicit pattern.  */
    if (r <= 18ULL)          return (seq_t)1;   /* 19 ones */
    if (r <= 106ULL)         return (seq_t)0;   /* 88 zeros     */
    if (r <= 125ULL)         return (seq_t)-1;  /* 19 minus ones */
    return (seq_t)0;                            /* remaining zeros */
}
