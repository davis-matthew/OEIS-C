/*
 * A078260  a(n) = denominator(N), where N = 0.246...(2n) is the concatenation
 *          of the first n even numbers after decimal point.
 * https://oeis.org/A078260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, frac, nonn, easy
 * Formula:   a(n) = denominator of 0.246...(2n) where the fractional part is
 *            the concatenation of the first n even numbers. The denominator
 *            is 10^L / gcd(S,10^L), with S the concatenated integer and L its
 *            digit length
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/seq_pow.h"   /* provides seq_pow(seq_t, idx_t) */
#include "../utils/seq_mul.h"   /* provides seq_mul(seq_t, seq_t) */

seq_t a(idx_t n)
{
    if (n == 0)          /* defined for n≥1, but guard */
        return 0;

    seq_t S = 0;         /* concatenated integer */
    idx_t total_digits = 0;

    /* Pre‑compute powers of 10 up to 5 (max digits of 2n for n≤10000) */
    static seq_t pow10[6];
    static bool init = false;
    if (!init) {
        pow10[0] = 1;
        for (int i = 1; i < 6; ++i)
            pow10[i] = seq_mul(pow10[i - 1], (seq_t)10);
        init = true;
    }

    for (idx_t i = 1; i <= n; ++i) {
        /* Even number 2*i */
        uint64_t val_u = (uint64_t)2 * i;

        /* Compute number of decimal digits of val_u */
        unsigned len = 1;
        uint64_t tmp = val_u;
        while (tmp >= 10) {
            tmp /= 10;
            ++len;
        }

        /* Update concatenated integer: S = S * 10^len + val_u */
        S = seq_mul(S, pow10[len]) + (seq_t)val_u;
        total_digits += (idx_t)len;
    }

    /* Compute v2 = exponent of 2 dividing S */
    idx_t v2 = 0;
    seq_t t = S;
    while ((t % 2) == 0) {
        t /= 2;
        ++v2;
    }

    /* Compute v5 = exponent of 5 dividing S */
    idx_t v5 = 0;
    t = S;
    while ((t % 5) == 0) {
        t /= 5;
        ++v5;
    }

    /* Denominator = 2^(L-v2) * 5^(L-v5) */
    idx_t exp2 = (total_digits >= v2) ? total_digits - v2 : 0;
    idx_t exp5 = (total_digits >= v5) ? total_digits - v5 : 0;

    seq_t denom = seq_pow((seq_t)2, exp2);
    denom = seq_mul(denom, seq_pow((seq_t)5, exp5));

    return denom;
}
