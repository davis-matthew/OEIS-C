/*
 * A049544  Primes p such that x^12 = 2 has a solution mod p.
 * https://oeis.org/A049544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A computed sequence: primes p such that x^12 ≡ 2 (mod p) has a solution. */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"

/* Helper: Euclidean gcd for unsigned 64‑bit integers. */
static uint64_t gcd_u64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    idx_t cnt = 0;
    for (idx_t p = 2; ; ++p) {
        if (!is_prime(p))
            continue;

        bool ok;
        if (p == 2) {
            ok = true;
        } else {
            uint64_t pm1 = (uint64_t)(p - 1);
            uint64_t d    = gcd_u64(12ULL, pm1);
            uint64_t exp  = pm1 / d;
            seq_t   res   = mod_pow((seq_t)2, exp, (seq_t)p);
            ok = (res == (seq_t)1);
        }

        if (ok) {
            ++cnt;
            if (cnt == n)
                return (seq_t)p;
        }
    }
}
