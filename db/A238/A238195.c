/*
 * A238195  Triangle of the discriminant of the polynomials x^n - x^m - 1,
 *          where 0 < m < n.
 * https://oeis.org/A238195
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = discriminant of the polynomial x^k - x^l - 1 where the
 *            pair (k,l) is obtained by lexicographic enumeration of the
 *            triangle with k = 2,3,4,... and l = 1,2,...,k-1. For a given
 *            index n (n ≥ 2) let pos = n - 1 k = smallest integer such that
 *            k*(k-1)/2 ≥ pos l = pos - (k-1)*(k-2)/2 Let d = gcd(...
 * Range:     i8 n <= 4, i16 n <= 11, i32 n <= 37, i64 n <= 106, i128 n <= 326,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_pow.h"   /* seq_pow(seq_t base, idx_t exp) */

/* Compute gcd for 64‑bit unsigned integers */
static idx_t gcd_uint64(idx_t a, idx_t b)
{
    while (b) {
        idx_t tmp = a % b;
        a = b;
        b = tmp;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;   /* undefined below offset, return 0 */

    /* Find pair (k,l) for this index n using simple linear search.
       n ≤ 10000 → at most 10000 iterations, negligible for time. */
    idx_t pos = n - 1;             /* position in 1‑based enumeration */
    idx_t k = 2;
    while ((k * (k - 1)) / 2 < pos) {
        ++k;
    }
    idx_t l = pos - ((k - 1) * (k - 2)) / 2;

    /* Basic quantities for the discriminant formula */
    idx_t u = k - l;
    idx_t d = gcd_uint64(l, u);
    idx_t t = k / d;
    idx_t exp_u = u / d;
    idx_t exp_l = l / d;

    /* k^t */
    seq_t k_pow_t = seq_pow((seq_t)k, t);

    /* B = u^(u/d) * l^(l/d) */
    seq_t u_pow  = seq_pow((seq_t)u, exp_u);
    seq_t l_pow  = seq_pow((seq_t)l, exp_l);
    seq_t B = u_pow * l_pow;

    /* c = (-1)^(exp_u) * B */
    seq_t c = B;
    if (exp_u & 1) {
        c = -c;
    }

    /* k^t - c */
    seq_t diff = k_pow_t - c;

    /* (k^t - c)^d */
    seq_t pow_d = seq_pow(diff, d);

    /* sign = (-1)^((k-1)*(k+2)/2) */
    idx_t sign_exp = ((k - 1) * (k + 2)) / 2;
    seq_t result = pow_d;
    if (sign_exp & 1) {
        result = -result;
    }

    return result;
}
