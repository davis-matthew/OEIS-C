/*
 * A127810  Least positive primitive root of (n-th prime)^5.
 * https://oeis.org/A127810
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   factor_u64, get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Rough formula: a(n) = least positive primitive root modulo (p_n)^5,
   where p_n is the n-th prime.  For n≥2. */
#include <stdint.h>
#include <stdbool.h>

#include "../utils/get_prime.h"
#include "../utils/factor_u64.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* The OEIS table starts with a(2), so n must be at least 2. */
    if (n < 2)
        return 0;          /* undefined; caller will not use such a value */

    /* get the n‑th prime */
    uint64_t p_u64 = get_prime(n);
    if (p_u64 <= 2)
        return 0;          /* p=2 is not needed for n≥2 according to the sequence */

    /* factor p-1 to test primitivity modulo p */
    uint64_t fac[64], exp[64];
    size_t fac_cnt;
    factor_u64(p_u64 - 1, fac, exp, &fac_cnt);

    /* Search the smallest g>1 with the required properties. */
    for (uint64_t g = 2;; ++g) {
        /* skip multiples of p */
        if (g % p_u64 == 0)
            continue;

        /* primitive root modulo p : check p-1 divisors */
        bool ok = true;
        for (size_t i = 0; i < fac_cnt && ok; ++i) {
            uint64_t q = fac[i];
            /* base = g mod p, exponent = (p-1)/q */
            seq_t r = mod_pow((seq_t)g, (p_u64 - 1) / q, (seq_t)p_u64);
            if (r == 1)
                ok = false;
        }
        if (!ok)
            continue;

        /* lift condition: g^(p-1) mod p^2 must not be 1 */
        seq_t mod_p2 = (seq_t)(p_u64 * p_u64);
        seq_t r2 = mod_pow((seq_t)g, p_u64 - 1, mod_p2);
        if (r2 == 1)
            continue;

        /* g satisfies all conditions: it is a primitive root modulo p^5. */
        return (seq_t)g;
    }
}
