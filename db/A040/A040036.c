/*
 * A040036  Primes p such that x^3 = 3 has a solution mod p.
 * https://oeis.org/A040036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 77 terms through n = 994 (2026-11-03)
 * Helpers:   is_prime, mem, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n-th prime p for which the congruence
 * x^3 ≡ 3 (mod p) has a solution.  For an odd prime p≠3 a solution
 * exists iff p ≡ 2 (mod 3) or, when p ≡ 1 (mod 3),
 * 3^((p−1)/3) ≡ 1 (mod p).  The prime p=3 is also admissible.
 * The function builds the sequence on demand and caches the results,
 * so repeated calls with arbitrary n are efficient. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/is_prime.h"
#include "../utils/powmod.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* undefined, but return 0 */

    static seq_t *seq = NULL;  /* cached sequence values */
    static size_t seq_len = 0; /* number of values stored */
    static size_t seq_cap = 0; /* capacity of the array */
    static idx_t next_candidate = 2; /* next integer to test for primality */

    /* grow the cached array when needed */
    if (seq_len == 0) {
        seq_cap = 32;
        seq = mem_new(seq_t, seq_cap);
    }

    /* keep generating primes until we have at least n terms */
    while (seq_len < (size_t)n) {
        /* find next prime */
        while (!is_prime(next_candidate)) {
            ++next_candidate;
        }

        /* check the cubic condition */
        bool ok = false;
        if (next_candidate == 3) {
            ok = true;
        } else if (next_candidate % 3 == 2) {
            ok = true;
        } else {
            /* test 3^((p-1)/3) mod p == 1 */
            seq_t res = powmod((seq_t)3,
                                (uint64_t)((next_candidate - 1) / 3),
                                (seq_t)next_candidate);
            ok = (res == (seq_t)1);
        }

        if (ok) {
            /* store the prime in the cache */
            if (seq_len >= seq_cap) {
                seq_cap = seq_cap * 2 + 16;
                seq = mem_resize(seq_t, seq, seq_cap);
            }
            seq[seq_len++] = (seq_t)next_candidate;
        }

        ++next_candidate;  /* move to the next integer for the next iteration */
    }

    return seq[n - 1];  /* sequence is one‑based */
}
