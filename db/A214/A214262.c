/*
 * A214262  Expansion of eta(q)^5 * eta(q^3) * eta(q^6)^4 / eta(q^2)^4 in
 *          powers of q.
 * https://oeis.org/A214262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult, changed
 * Formula:   not stated
 * Range:     i8 n <= 12, i16 n <= 161, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   local_value_ll
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A214262
 *  Multiplicative function a(n):
 *  a(1) = 1
 *  For prime power p^e:
 *     a(2^e)   = -(4^(e+1) + 9*(-1)^(e+1)) / 5
 *     a(3^e)   = 9^e
 *     a(p^e)   = ((p^2)^(e+1) – 1) / (p^2 – 1)   if p ≡ 1 (mod 6)
 *     a(p^e)   = ((p^2)^(e+1) – (-1)^(e+1)) / (p^2 + 1)  if p ≡ 5 (mod 6)
 *
 *  This implementation factors n by trial division (sufficient for n ≤ 10 000)
 *  and evaluates the product of the local factors using 64‑bit arithmetic.
 *  The final result is cast to seq_t.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/local_value_ll.h"

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    uint64_t nn = (uint64_t)n;
    long long result = 1;

    for (uint64_t p = 2; p * p <= nn; ++p) {
        if (nn % p == 0) {
            uint64_t e = 0;
            while (nn % p == 0) {
                nn /= p;
                ++e;
            }
            result *= local_value_ll(p, e);
        }
    }
    if (nn > 1) {                     /* remaining prime factor */
        result *= local_value_ll(nn, 1);
    }
    return (seq_t)result;
}
