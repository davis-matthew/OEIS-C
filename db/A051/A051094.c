/*
 * A051094  Primes p such that x^50 = -2 has a solution mod p.
 * https://oeis.org/A051094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _gcd_u64, _is_50th_power_residue, _mod_pow, isPrime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A051094: primes p such that the congruence x^50 ≡ –2 (mod p) has a solution. 
    For a prime p > 2 the congruence has a solution iff (–2)^((p‑1)/gcd(p‑1,50)) ≡ 1 (mod p).
    The special case p = 2 is always valid (take x = 0). 
    The function iterates over primes until the n‑th suitable prime is found.   */

#include <stdint.h>
#include <stdbool.h>

#include "../utils/isPrime.h"   /* bool isPrime(idx_t x); */
#include "../utils/_gcd_u64.h"
#include "../utils/_is_50th_power_residue.h"
#include "../utils/_mod_pow.h"

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
        if (isPrime(candidate)) {
            if (_is_50th_power_residue((seq_t)candidate)) {
                ++count;
                if (count == n)
                    return (seq_t)candidate;
            }
        }
        ++candidate;
    }
}
