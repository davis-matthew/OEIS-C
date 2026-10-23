/*
 * A162906  a(n) = n - A081707(n).
 * https://oeis.org/A162906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + A001222(n) - A000005(n)
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute a(n) = n + Ω(n) - d(n)  where
   Ω(n) = sum of exponents in prime factorisation of n
   d(n) = number of divisors of n.
   For n = 1 we treat Ω(1) = 0 and d(1) = 1, giving a(1)=0. */
seq_t a(idx_t n) {
    /* d(n) starts at 1, Ω(n) starts at 0 */
    seq_t d = (seq_t)1;
    idx_t omega = 0;
    idx_t m = n;
    idx_t p = 2;

    /* trial‑division factorisation */
    while (p * p <= m) {
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) {
                m /= p;
                e++;
                omega++;
            }
            d *= (seq_t)(e + 1);   /* update divisor count */
        }
        /* proceed to next potential factor: 3,5,7,...  */
        p += (p == 2 ? 1 : 2);
    }

    /* if remainder is a prime > 1 */
    if (m > 1) {
        omega++;
        d *= (seq_t)2;           /* exponent 1 → factor (1+1) */
    }

    /* a(n) = n + Ω(n) - d(n) */
    return (seq_t)n + (seq_t)omega - d;
}
