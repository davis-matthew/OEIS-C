/*
 * A128581  Expansion of (phi(q^2) * phi(-q^3) - phi(-q) * phi(q^6)) / 2 in
 *          powers of q where phi() is a Ramanujan theta function.
 * https://oeis.org/A128581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   not stated
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS formula: a(n) = product_p a(p^{e_p}) with
 *   a(2^e)   = (-1)^{e+1}
 *   a(3^e)   = (-1)^e
 *   a(p^e)   = e+1            if p ≡ 1 or 7 (mod 24)
 *   a(p^e)   = (e+1)(-1)^e    if p ≡ 5 or 11 (mod 24)
 *   a(p^e)   = (1+(-1)^e)/2   if p ≡ 13,17,19,23 (mod 24)
 *  a(1) = 1.  The sequence is multiplicative.               */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* index 1 is the first term; n is unsigned */
    if (n <= 1) return 1;  /* a(1) = 1, undefined for 0 but not needed */

    /* factorize n: prime factors in primes[], exponents in exps[] */
    /* Max number of distinct prime factors is < 64 for uint64_t */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t term = 1;

        if (p == 2) {
            /* a(2^e) = (-1)^{e+1} */
            term = (e & 1) ? 1 : -1;
        }
        else if (p == 3) {
            /* a(3^e) = (-1)^e */
            term = (e & 1) ? -1 : 1;
        }
        else {
            uint64_t mod = p % 24;
            if (mod == 1 || mod == 7) {
                term = (seq_t)(e + 1);
            }
            else if (mod == 5 || mod == 11) {
                seq_t sign = (e & 1) ? -1 : 1;
                term = (seq_t)(e + 1) * sign;
            }
            else if (mod == 13 || mod == 17 || mod == 19 || mod == 23) {
                term = (seq_t)((e & 1) ? 0 : 1);
            }
            else {
                /* Should not happen for primes > 3 */
                term = 1;
            }
        }

        result *= term;
        if (result == 0) break;
    }

    return result;
}
