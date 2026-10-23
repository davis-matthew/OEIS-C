/*
 * A130425  a(n) = numerator of Sum_{k=1..n} 1/k^(n+1-k).
 * https://oeis.org/A130425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = numerator of Sum_{k=1..n} 1/k^(n+1-k)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   gcd, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/pow_int.h"

seq_t a(idx_t n) {
    seq_t num = 0;
    seq_t den = 1;

    for (idx_t k = 1; k <= n; ++k) {
        idx_t exp = n + 1 - k;
        seq_t kpow = pow_int((seq_t)k, exp);          /* k^exp */

        seq_t g   = gcd(den, kpow);                  /* gcd(den, kpow) */
        seq_t lden = den / g * kpow;                 /* lcm(den, kpow) */

        seq_t new_num = num * (kpow / g) + den / g;  /* num/den + 1/kpow */
        seq_t new_den = lden;

        seq_t g2 = gcd(new_num, new_den);            /* reduce fraction */
        num  = new_num / g2;
        den  = new_den / g2;
    }

    return num;  /* numerator of the reduced fraction */
}
