/*
 * A138949  Expansion of (3 * phi(q^3)^2 - phi(q)^2) / 2 in powers of q where
 *          phi() is a Ramanujan theta function.
 * https://oeis.org/A138949
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -2 * Π_{p^e || n} b(p^e) with b(2^e) = 1, b(3^e) = -1 +
 *            2*(-1)^e, b(p^e) = e+1 if p≡1 or 5 (mod 12), b(p^e) =
 *            (1+(-1)^e)/2 if p≡7 or 11 (mod 12). a(0)=1
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    /* Temporary storage for prime factors and exponents. 128 distinct
       prime factors are more than enough for 64‑bit numbers. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t b = 1;                     /* product of b(p^e) values */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t val = 1;

        if (p == 2) {
            /* b(2^e) = 1 */
            val = 1;
        } else if (p == 3) {
            /* b(3^e) = -1 + 2*(-1)^e */
            val = -1 + 2 * ((e % 2 == 0) ? 1 : -1);
        } else {
            int mod12 = (int)(p % 12);
            if (mod12 == 1 || mod12 == 5) {
                /* b(p^e) = e+1 for p≡1,5 mod 12 */
                val = (seq_t)(e + 1);
            } else { /* mod12 is 7 or 11 */
                /* b(p^e) = (1+(-1)^e)/2 */
                val = (e % 2 == 0) ? (seq_t)1 : (seq_t)0;
            }
        }

        b *= val;
        if (b == 0) break;          /* zero propagates */
    }

    return -2 * b;                   /* a(n) = -2 * b(n) */
}
