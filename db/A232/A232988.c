/*
 * A232988  Numerators of coefficients in q-expansion of Sum_{n = -oo,oo, n !=
 *          0}
 *          (((-1)^(n-1)/(2*n^2))*(sin(n*Pi/3)+sin(2*n*Pi/3))/(1-q^n))/sqrt(3),
 *          excluding the constant term.
 * https://oeis.org/A232988
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = Σ_{d|n, d≡1 (mod 6)} 1/d^2 – Σ_{d|n, d≡5 (mod 6)} 1/d^2
 *            The function returns the numerator of this fraction in lowest
 *            terms
 * Range:     u8 n <= 16, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   divisor_sign, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/divisor_sign.h"

/* Signed 128‑bit integer for intermediate arithmetic.  GCC/Clang support it; 
   for builds that use smaller types the final result will fit into seq_t. */
typedef __int128 int128_t;

/* absolute value for int128_t */
static inline int128_t int128_abs(int128_t x) { return x < 0 ? -x : x; }

/* Euclidean gcd for int128_t */
static inline int128_t int128_gcd(int128_t a, int128_t b) {
    a = int128_abs(a);
    b = int128_abs(b);
    while (b != 0) {
        int128_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* add the rational term sign/d^2 to the running fraction (num/den) */
static inline void add_term(int128_t *num, int128_t *den, idx_t d, int sign) {
    if (sign == 0) return;
    int128_t d_sq = (int128_t)d * d;
    int128_t new_num = (*num) * d_sq + (int128_t)sign * (*den);
    int128_t new_den = (*den) * d_sq;
    int128_t g = int128_gcd(new_num, new_den);
    *num = new_num / g;
    *den = new_den / g;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;

    int128_t num = 0;   /* numerator (will be positive) */
    int128_t den = 1;   /* denominator (always positive) */

    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            /* divisor d */
            int s = divisor_sign(d);
            add_term(&num, &den, d, s);

            /* paired divisor n/d (avoid double counting squares) */
            idx_t e = n / d;
            if (e != d) {
                s = divisor_sign(e);
                add_term(&num, &den, e, s);
            }
        }
    }

    /* num is guaranteed to be positive */
    return (seq_t)num;
}
