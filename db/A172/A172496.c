/*
 * A172496  a(n) = numerator of fraction a/b, where gcd(a, b) = 1, whose
 *          decimal representation has the form 0.(1)(2)(3)...(n-1)(n)... with
 *          period (1)(2)(3)...(n-1)(n).
 * https://oeis.org/A172496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = numerator of the fraction with decimal period * formed by
 *            concatenating the integers 1,2,…,n. * Let P be that concatenated
 *            integer and L the total number of digits. * Then the fraction
 *            equals P/(10^L−1) reduced to lowest terms. * a(n) = P / gcd(P,
 *            10^L−1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 23,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

/* Compute 10^exp mod mod_val for seq_t.  exp fits in 64 bits. */
static seq_t mod_pow_10(idx_t exp, seq_t mod_val)
{
    seq_t result = 1;
    seq_t base   = 10 % mod_val;
    idx_t e = exp;
    while (e > 0) {
        if (e & 1) result = (result * base) % mod_val;
        base = (base * base) % mod_val;
        e >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;         /* below offset – undefined */

    seq_t P = 0;                 /* concatenated number 1…n */
    idx_t digit = 1;             /* digits of current k */
    idx_t next_change = 10;      /* next k where digit increases */
    seq_t pow10d = 10;           /* 10^digit as seq_t */
    idx_t len = 0;               /* total digits of P */

    for (idx_t k = 1; k <= n; ++k) {
        if (k == next_change) {
            ++digit;
            pow10d *= 10;
            next_change *= 10;
        }
        P = P * pow10d + (seq_t)k;
        len += digit;
    }

    seq_t r = mod_pow_10(len, P);      /* 10^len mod P */
    seq_t r1 = (r == 0) ? (P - 1) : (r - 1);  /* (10^len - 1) mod P */

    seq_t g = gcd(P, r1);   /* GCD(P, 10^len-1) */
    return P / g;           /* numerator in lowest terms */
}
