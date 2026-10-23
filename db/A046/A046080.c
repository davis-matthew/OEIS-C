/*
 * A046080  a(n) is the number of integer-sided right triangles with
 *          hypotenuse n.
 * https://oeis.org/A046080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/2)*(Π_{p≡1 (mod 4)} (2*e_p + 1) – 1)
 * Range:     u8 n <= 9069, u16 n <= 9069, u32 n <= 9069, u64 n <= 9069,
 *            u128 n <= 9069, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

/* Return the number of integer‑sided right triangles with
   hypotenuse n.  n >= 1. */
seq_t a(idx_t n)
{
    /* Factor n.  We need the distinct prime factors and their exponents. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t  count = 64;           /* capacity */
    factor_u64((uint64_t)n, primes, exps, &count);

    seq_t prod = (seq_t)1;        /* product over primes ≡ 1 (mod 4) */

    for (size_t i = 0; i < count; ++i) {
        uint64_t p   = primes[i];
        uint64_t exp = exps[i];

        if (p == 2)            /* 2 is ignored */
            continue;

        if (p & 3U) {          /* p ≡ 1 or 3 (mod 4) */
            if ((p & 3U) == 1U) /* p ≡ 1 (mod 4) */
                prod = prod * ((seq_t)(2 * exp + 1));
            /* p ≡ 3 (mod 4) is ignored – it never contributes to the product */
        }
    }

    /* If no prime ≡ 1 (mod 4) occurred, prod == 1 and a(n) == 0. */
    if (prod == (seq_t)1)
        return (seq_t)0;

    /* a(n) = (prod - 1) / 2. */
    return (prod - (seq_t)1) / (seq_t)2;
}
