/*
 * A226786  If n=0 (mod 2) then a(n)=0, otherwise a(n)=8^(-1) in Z/nZ*.
 * https://oeis.org/A226786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 if n is even; otherwise a(n) is the inverse of 8 (mod
 *            n)
 * Range:     u8 n <= 296, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mod_pow.h"

/* Euler's totient function for 64‑bit integers. */
static idx_t phi_u64(idx_t n)
{
    idx_t result = n;
    idx_t temp = n;
    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            while (temp % p == 0)
                temp /= p;
            result -= result / p;
        }
    }
    if (temp > 1)
        result -= result / temp;
    return result;
}

seq_t a(idx_t n)
{
    if ((n & 1ULL) == 0)
        return 0;

    idx_t phi = phi_u64(n);          /* φ(n) ≤ n−1, fits in idx_t. */
    idx_t exp = phi - 1;             /* 8^(φ(n)−1) ≡ 8^(−1) (mod n). */

    return mod_pow((seq_t)8, exp, (seq_t)n);
}
