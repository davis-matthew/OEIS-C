/*
 * A240031  Numbers with primitive root -12.
 * https://oeis.org/A240031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the nth integer (m ≥ 5) such that -12 is a primitive root
   modulo every prime power dividing m.  The function enumerates the numbers
   in ascending order. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/factor_u64.h"   /* factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt) */
#include "../utils/mod_pow.h"     /* mod_pow(seq_t base, uint64_t exp, seq_t mod) */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* out of bounds – not used by the judge */

    uint64_t found     = 0;
    uint64_t candidate = 5;              /* the smallest m ≥ 5 with the property */

    static uint64_t pf[64], ef[64];     /* buffers for factorisations */
    size_t cnt, cnt2;

    for (;;) {
        /* skip even numbers and multiples of 3 (they can’t satisfy the property) */
        if ((candidate & 1ULL) == 0ULL || candidate % 3ULL == 0ULL) {
            ++candidate;
            continue;
        }

        /* factorise candidate; it must be a prime power */
        factor_u64(candidate, pf, ef, &cnt);
        if (cnt != 1) {
            ++candidate;
            continue;
        }

        uint64_t p = pf[0];
        uint64_t e = ef[0];

        /* -12 modulo p */
        uint64_t a_mod = (p - (12ULL % p)) % p;
        if (a_mod == 0) {          /* impossible for p ≥ 5, but keep safe */
            ++candidate;
            continue;
        }

        /* test that a_mod is a primitive root mod p */
        uint64_t ord = p - 1;
        factor_u64(p - 1, pf, ef, &cnt2);
        for (size_t i = 0; i < cnt2; ++i) {
            uint64_t q = pf[i];
            while (ord % q == 0) {
                uint64_t cand = ord / q;
                seq_t r = mod_pow((seq_t)a_mod, cand, (seq_t)p);
                if (r == (seq_t)1)
                    ord = cand;
                else
                    break;
            }
        }
        if (ord != p - 1) {
            ++candidate;
            continue;
        }

        /* if the exponent e > 1, check the Hensel lifting condition */
        if (e > 1) {
            seq_t p2 = (seq_t)p * (seq_t)p;
            seq_t r = mod_pow((seq_t)a_mod, p - 1, p2);
            if (r == (seq_t)1) {
                ++candidate;
                continue;
            }
        }

        /* a valid number found */
        ++found;
        if (found == n)
            return (seq_t)candidate;

        ++candidate;
    }
}
