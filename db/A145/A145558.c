/*
 * A145558  Denominators of partial sums of a certain alternating series of
 *          inverse central binomial coefficients.
 * https://oeis.org/A145558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = (1/2)*denominator( sum_{k=1..n}
 *            (-1)^{k+1}/(k*binomial(2k,k)) )
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/binomial.h"

#include <stdint.h>

/* Add a signed rational term term_sign*term_num/term_den to the
   current rational *N/'*D with sign *sign.
   After the call, *N, *D and *sign are updated to the sum in
   lowest terms (denominator positive).  */
static inline void add_signed_fraction(seq_t *N, seq_t *D, int *sign,
                                       int term_sign, seq_t term_num,
                                       seq_t term_den)
{
    /* Zero case */
    if (*N == 0 && *sign == 0) {
        *N  = term_num;
        *D  = term_den;
        *sign = term_sign;
        return;
    }

    /* Compute lcm of the denominators: lcm = (*D / gcd(*D,term_den)) * term_den */
    seq_t G = gcd(*D, term_den);
    seq_t D_div_G      = *D / G;
    seq_t term_den_div_G = term_den / G;
    seq_t lcm = D_div_G * term_den;  /* lcm = (*D/G) * term_den */

    /* Scale numerators to common denominator */
    seq_t a = (*N) * term_den_div_G;          /* current * (term_den/G) */
    seq_t b = term_num * D_div_G;             /* term_num * (*D/G) */

    int s1 = *sign;
    int s2 = term_sign;
    seq_t x;
    int res_sign;

    if (s1 == s2) {           /* same sign → sum */
        x = a + b;
        res_sign = s1;
    } else {                  /* opposite signs → difference */
        if (a >= b) {
            x = a - b;
            res_sign = s1;
        } else {
            x = b - a;
            res_sign = s2;
        }
    }

    if (x == 0) {              /* result is zero */
        *N = 0;
        *D = 1;
        *sign = 0;
    } else {                   /* reduce the fraction */
        seq_t g = gcd(x, lcm);
        *N = x / g;
        *D = lcm / g;
        *sign = res_sign;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n<1, return 0 */

    /* current rational a/b with sign */
    seq_t N = 0;   /* numerator >= 0 */
    seq_t D = 1;   /* denominator > 0 */
    int sign = 0;  /* 0 for zero, +1 or -1 for non‑zero */

    seq_t central = (seq_t)2;  /* C(2,1) for k=1 */

    for (idx_t k = 1; k <= n; ++k) {
        if (k > 1) {
            /* recurrence: C(2k,k) = C(2k-2,k-1) * (4k-2) / k */
            central = (central * (seq_t)(4 * k - 2)) / (seq_t)k;
        }

        seq_t term_den = (seq_t)k * central;    /* k * C(2k,k) */
        int   term_sign = (k & 1) ? 1 : -1;     /* (-1)^(k+1) */

        /* term numerator is 1 in absolute value */
        add_signed_fraction(&N, &D, &sign, term_sign, (seq_t)1, term_den);
    }

    /* a(n) = denominator(r(n)) / 2 */
    return D / (seq_t)2;
}
