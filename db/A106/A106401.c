/*
 * A106401  Expansion of (eta(q) * eta(q^9))^3 / eta(q^3)^2 in powers of q.
 * https://oeis.org/A106401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   not stated
 * Range:     i8 n <= 12, i16 n <= 210, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   power_seq_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A106401: multiplicative function defined by
 *
 *     a(1) = 1.
 *     a(3^e) = 0 for e>0.
 *     for odd prime powers p^e (p ≠ 3):
 *         f = (p^(e+1)-1)/(p-1)    (sum of geometric series)
 *         if e is odd AND p ≡ 2 (mod 3)  set f = -f.
 *     a(n) = ∏ f over all prime powers dividing n.
 *
 *  The function below implements this definition using trial division.
 */

#include <stdint.h>
#include "../utils/power_seq_u64.h"

seq_t a(idx_t n) {
    if (n == 0 || n % 3 == 0) /* a(1)=1 but n%3==0 covers 3,9,27... */
        return (n == 0 ? 0 : (n % 3 == 0 ? 0 : 1));

    seq_t result = 1;
    idx_t temp = n;

    /* handle factor 2 */
    int e = 0;
    while (!(temp & 1)) { /* temp % 2 == 0 */
        temp >>= 1;      /* temp /= 2 */
        e++;
    }
    if (e > 0) {
        seq_t p = 2;
        seq_t p_pow = 1;
        for (int i = 0; i < e; ++i) p_pow *= p;      /* p^e */
        seq_t numerator = p_pow * p - 1;              /* p^(e+1)-1 */
        seq_t denom = p - 1;
        seq_t f = numerator / denom;
        if ((e & 1) && (p % 3 == 2)) f = -f;
        result *= f;
    }

    /* handle odd factors */
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p != 0) continue;
        e = 0;
        while (temp % p == 0) {
            temp /= p;
            e++;
        }
        seq_t p_pow = 1;
        for (int i = 0; i < e; ++i) p_pow *= (seq_t)p;      /* p^e */
        seq_t numerator = p_pow * (seq_t)p - 1;               /* p^(e+1)-1 */
        seq_t denom = (seq_t)p - 1;
        seq_t f = numerator / denom;
        if ((e & 1) && (p % 3 == 2)) f = -f;
        result *= f;
    }

    /* if temp > 1 it is a prime > sqrt(original n) */
    if (temp > 1) {
        seq_t p = (seq_t)temp;
        e = 1;
        seq_t numerator = p * p - 1;  /* p^(1+1)-1 */
        seq_t denom = p - 1;
        seq_t f = numerator / denom;
        if ((e & 1) && (p % 3 == 2)) f = -f;
        result *= f;
    }

    return result;
}
