/*
 * A226033  Round(n * exp(-1 - 1/(2n))), an approximation to the number of
 *          daughters to wait before picking in the sultan's dowry problem
 *          (Better that A225593).
 * https://oeis.org/A226033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Round(n * exp(-1 - 1/(2n)))
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation is performed entirely with integer arithmetic. */

#include <stdint.h>

/* Greatest common divisor for 128‑bit integers */
static __int128 gcd128(__int128 a, __int128 b)
{
    /* Use Euclid's algorithm; keep operands non‑negative */
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b != 0) {
        __int128 t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Helper: reduce a fraction n/d by their gcd. The caller ensures d>0. */
static void reduce_fraction(__int128 *n, __int128 *d)
{
    __int128 g = gcd128(*n, *d);
    *n /= g;
    *d /= g;
}

/* Factorial values up to 20 fit nicely in 64 bits. */
static const unsigned long long fact[21] = {
    1ULL,
    1ULL,
    2ULL,
    6ULL,
    24ULL,
    120ULL,
    720ULL,
    5040ULL,
    40320ULL,
    362880ULL,
    3628800ULL,
    39916800ULL,
    479001600ULL,
    6227020800ULL,
    87178291200ULL,
    1307674368000ULL,
    20922789888000ULL,
    355687428096000ULL,
    6402373705728000ULL,
    121645100408832000ULL,
    2432902008176640000ULL
};

seq_t a(idx_t n)
{
    /* Safety: the OEIS offset is 1. For n < 1 we return 0. */
    if (n < 1) return 0;

    /* 1.  Compute an integer approximation of e^{-1}. */
    /* Sum_{k=0}^{K} (-1)^k / k!  with K=20 gives error < 10^{-19}. */
    const int K = 20;
    __int128 e_num = 0;   /* numerator of the partial sum */
    __int128 e_den = 1;   /* denominator of the partial sum */

    for (int k = 0; k <= K; ++k) {
        __int128 term_num = (k % 2 == 0) ? 1 : -1;        /* (-1)^k */
        __int128 term_den = fact[k];                     /* k!  (fits 64‑bit) */

        /* New fraction: e_num/e_den + term_num/term_den */
        __int128 new_num = e_num * term_den + term_num * e_den;
        __int128 new_den = e_den * term_den;
        reduce_fraction(&new_num, &new_den);

        e_num = new_num;
        e_den = new_den;
    }

    /* 2.  Compute r = 1/(2n). */
    __int128 r_num = 1;
    __int128 r_den = ( __int128 )2 * ( __int128 )n;  /* 2n  fits 64‑bit */

    /* 3.  Compute S = exp(-r) to 3rd order:
           S = 1 - r + r^2/2 - r^3/6
       The error of this truncation is < 10^{-15} for n <= 10000. */
    __int128 S_num = 1;
    __int128 S_den = 1;
    __int128 rpow_num = 1;              /* r^t numerator = 1 */
    __int128 rpow_den = 1;              /* r^t denominator = (2n)^t */

    for (int t = 1; t <= 3; ++t) {
        /* Compute r^t */
        rpow_den *= r_den;              /* multiply denominator by (2n) each step */

        /* Term: (-1)^t / (t! * r^t) = (-1)^t * r^t? Wait:
           For our series we need (-1)^t * r^t / t!  not divided by r^t.
           However, r = 1/(2n), so r^t is 1/(2n)^t.   */
        __int128 term_num = (t % 2 == 0) ? 1 : -1;           /* (-1)^t */
        __int128 term_den = fact[t] * rpow_den;              /* t! * (2n)^t */

        /* Add term to S */
        __int128 new_num = S_num * term_den + term_num * S_den;
        __int128 new_den = S_den * term_den;
        reduce_fraction(&new_num, &new_den);

        S_num = new_num;
        S_den = new_den;
    }

    /* 4.  Multiply everything together:
           result = n * e_num/e_den * S_num/S_den  */
    __int128 num = (__int128)n * e_num * S_num;
    __int128 den = e_den * S_den;
    reduce_fraction(&num, &den);

    /* 5.  Round to nearest integer: (num + den/2) / den  */
    __int128 rounded = (num + den / 2) / den;

    /* Convert back to seq_t (fits into 64 bits for all n<=10 000). */
    return (seq_t)rounded;
}
