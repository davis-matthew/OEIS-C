/*
 * A040064  Primes p such that x^3 = 12 has no solution mod p.
 * https://oeis.org/A040064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 60 terms through n = 828 (2026-11-03)
 * Helpers:   mem, next_prime, powmod_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of primes p>3 such that the congruence
   x^3 ≡ 12 (mod p) has no solution.  
   For p≠3, this happens iff p≡1 (mod 3) and 12 is a non‑cubic residue
   modulo p.  Equivalently
           12^((p‑1)/3) ≢ 1 (mod p)
   for such primes.  The function below generates these primes on
   demand, caching the results so that any order of calls to a(n) works
   efficiently. */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/next_prime.h"
#include "../utils/powmod_u64.h"
#include "../utils/mem.h"

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* static cache of sequence terms */
    static seq_t *seq = NULL;
    static size_t seq_len = 0, seq_cap = 0;
    /* last prime examined; start just after 3 */
    static uint64_t last_prime = 3;

    if (n == 0) return 0; /* undefined, but safe */

    /* fetch from cache if already computed */
    if (n <= seq_len)
        return seq[n-1];

    /* extend cache until we have at least n terms */
    while (seq_len < (size_t)n)
    {
        /* find next prime larger than last_prime */
        last_prime = next_prime(last_prime);
        /* skip primes that are not 1 mod 3 or are 2 or 3 */
        if (last_prime < 5 || (last_prime % 3) != 1)
            continue;

        /* check cubic residue condition:
           12^((p-1)/3) mod p != 1  */
        uint64_t exp = (last_prime - 1) / 3;
        uint64_t residue = powmod_u64(12, exp, last_prime);
        if (residue != 1)
        {
            /* add prime to cache */
            grow_seq(&seq, &seq_cap, seq_len + 1);
            seq[seq_len++] = (seq_t)last_prime;
        }
    }

    return seq[n-1];
}
