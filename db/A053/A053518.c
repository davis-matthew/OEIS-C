/*
 * A053518  Numerators of successive convergents to continued fraction
 *          1+2/(3+3/(4+4/(5+5/(6+6/(7+7/(8+8/(9+9/10+...))))))).
 * https://oeis.org/A053518
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, nice, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A053518
    a(n) = numerator of the nth convergent of
    1 + 2/(3 + 3/(4 + 4/(5 + 5/(6 + 6/(7 + 7/(8 + 8/(9 + 9/10 + ...)))))))
    Algorithm: compute the continued fraction from the bottom up, then
    evaluate 1 + 2/T_3 and reduce the fraction. */

#include <stdint.h>

/* Euclidean algorithm for whatever integer type seq_t supports % */
static inline seq_t gcd_seq_t(seq_t a, seq_t b)
{
    while (b) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* n == 0 -> convergent is simply 1 */
    if (n == 0) return (seq_t)1;

    /* Start from the deepest level: T_{n+2} = n+2 = p/q */
    seq_t p = (seq_t)(n + 2);   /* numerator of T_k */
    seq_t q = (seq_t)1;         /* denominator of T_k */

    /* Work backwards for k = n+1 down to 3 (inclusive) */
    for (idx_t k = n + 1; k > 2; --k) {
        seq_t new_p = (seq_t)k * (p + q);  /* k*(p+q) */
        seq_t new_q = p;                   /* denominator becomes previous numerator */
        p = new_p;
        q = new_q;
    }

    /* Now T_3 = p/q.  The convergent is 1 + 2/T_3 = (p + 2q)/p */
    seq_t numer = p + (seq_t)2 * q;   /* numerator before reduction */
    seq_t denom = p;                  /* denominator */

    /* Reduce the fraction and return the numerator */
    seq_t g = gcd_seq_t(numer, denom);
    return numer / g;
}
