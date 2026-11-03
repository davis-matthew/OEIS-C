/*
 * A051094  Primes p such that x^50 = -2 has a solution mod p.
 * https://oeis.org/A051094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 87 terms through n = 965 (2026-11-01)
 * Helpers:   gcd_u64, is_prime, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A051094: primes p such that the congruence x^50 ≡ –2 (mod p) has a solution. 
    For a prime p > 2 the congruence has a solution iff (–2)^((p‑1)/gcd(p‑1,50)) ≡ 1 (mod p).
    The special case p = 2 is always valid (take x = 0). 
    The function iterates over primes until the n‑th suitable prime is found.   */

#include <stdint.h>
#include <stdbool.h>

#include <stddef.h>
#include "../utils/gcd_u64.h"
#include "../utils/is_prime.h"
#include "../utils/powmod.h"

/* Determine whether a prime p satisfies the 50‑th power residue condition. */
static bool _is_50th_power_residue(seq_t p)
{
    /* Special case p = 2 : 0^50 ≡ 0 ≡ –2 (mod 2). */
    if (p == (seq_t)2)
        return true;

    uint64_t pm1 = (uint64_t)p - 1ULL;
    uint64_t d   = gcd_u64(pm1, 50ULL);
    uint64_t exp = pm1 / d;                 /* (p‑1)/gcd(p‑1,50) */

    seq_t base = (seq_t)p - 2;              /* –2 mod p  since p > 2 */
    seq_t val  = powmod(base, exp, p);

    return val == (seq_t)1;
}

/* ------------------------------------------------------------------ */
/* Simple gcd for 64‑bit integers.                                    */

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;                    /* Undefined for n = 0 */

    uint64_t candidate = 2;                /* Candidate prime (uint64_t for sieve) */
    idx_t    count     = 0;                /* Number of valid primes found */

    while (true) {
        if (is_prime(candidate)) {
            if (_is_50th_power_residue((seq_t)candidate)) {
                ++count;
                if (count == n)
                    return (seq_t)candidate;
            }
        }
        ++candidate;
    }
}
