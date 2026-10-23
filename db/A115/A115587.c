/*
 * A115587  a(n) = denominator of b(n), where b(1) = 1, b(n+1) = Sum_{k=1..n}
 *          b(k)^((-1)^(n-k)).
 * https://oeis.org/A115587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = denominator of b(n) for the sequence * b(1) = 1, * b(n+1)
 *            = Sum_{k=1..n} b(k)^{(-1)^(n-k)}, * where (-1)^(m) = 1 when m is
 *            even, -1 when m is odd. * The value a(n) is the denominator of
 *            b(n) in reduced form
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 10, u64 n <= 11, u128 n <= 12,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"   /* seq_t gcd(seq_t a, seq_t b); */

/* Rational addition:  (*n1/*d1) + (n2/d2)  →  (*n1/*d1)   */
static void add_frac(seq_t *n1, seq_t *d1, seq_t n2, seq_t d2)
{
    seq_t g = gcd(*d1, d2);
    seq_t lcm = (*d1 / g) * d2;           /* lcm of denominators */
    seq_t factor1 = lcm / *d1;
    seq_t factor2 = lcm / d2;
    seq_t new_num = (*n1) * factor1 + n2 * factor2;
    seq_t g2 = gcd(new_num, lcm);
    *n1 = new_num / g2;
    *d1 = lcm / g2;
}

/* a(idx_t n) – denominator of b(n). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out‑of‑range, not used in OEIS */
    if (n == 1) return 1;          /* b(1) = 1 */

    /* Keep track of four running sums of b(k) and 1/b(k).  */
    seq_t S_odd_num = 1,  S_odd_den = 1;  /* Σ_{k odd <= m}  b(k)      */
    seq_t R_odd_num = 1,  R_odd_den = 1;  /* Σ_{k odd <= m}  1/b(k)    */
    seq_t S_even_num = 0, S_even_den = 1; /* Σ_{k even <= m} b(k)      */
    seq_t R_even_num = 0, R_even_den = 1; /* Σ_{k even <= m} 1/b(k)    */

    seq_t bn_num = 0, bn_den = 1;   /* b(m) on the last iteration */

    for (idx_t m = 2; m <= n; ++m) {
        seq_t b_num, b_den;
        if ((m & 1) == 0) {         /* m even → use odd sums for b(m) */
            b_num = S_odd_num;
            b_den = S_odd_den;
            add_frac(&b_num, &b_den, R_even_num, R_even_den);
        } else {                    /* m odd  → use even sums for b(m) */
            b_num = S_even_num;
            b_den = S_even_den;
            add_frac(&b_num, &b_den, R_odd_num, R_odd_den);
        }

        bn_num = b_num; bn_den = b_den;          /* remember last b */

        /* Update the sums with the new term.  */
        if ((m & 1) == 0) {                     /* m even */
            add_frac(&S_even_num, &S_even_den, b_num, b_den);
            add_frac(&R_even_num, &R_even_den, b_den, b_num); /* reciprocal */
        } else {                                /* m odd  */
            add_frac(&S_odd_num, &S_odd_den, b_num, b_den);
            add_frac(&R_odd_num, &R_odd_den, b_den, b_num);   /* reciprocal */
        }
    }
    return bn_den;   /* denominator of b(n) in lowest terms */
}
