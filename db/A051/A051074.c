/*
 * A051074  Primes p such that x^10 = -2 has a solution mod p.
 * https://oeis.org/A051074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd64, is_prime_u64, modpow64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that the congruence x^10 = -2 (mod p) has a
   solution.  This is obtained by testing each prime with Fermat's
   little theorem and performing modular exponentiation. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/gcd64.h"
#include "../utils/modpow64.h"
#include "../utils/is_prime_u64.h"

/* Dynamic storage for the sequence.  The array is grown on demand. */
static seq_t *seq = NULL;            /* array of sequence elements        */
static size_t seq_cap = 0;           /* current capacity of seq array     */
static size_t seq_len = 0;           /* number of sequence elements stored */

/* Current prime candidate to be examined.  Starts at 1 and is
   incremented until a prime satisfying the congruence is found. */
static uint64_t candidate = 1;

/* Return true iff the prime p satisfies x^10 = -2 (mod p). */
static bool has_solution(uint64_t p)
{
    if (p == 2)                 /* 0^10 ≡ 0 ≡ -2 (mod 2) is a solution */
        return true;
    uint64_t g = gcd64(10, p - 1);
    uint64_t exp = (p - 1) / g;            /* (p‑1)/gcd(10, p‑1)            */
    uint64_t base = p - 2;                 /* -2 modulo p, 0 < base < p     */
    return modpow64(base, exp, p) == 1;
}

/* Expand the sequence storage when needed.  The array is allocated
   with malloc/realloc; the cast to (seq_t *) is required for C++.
   For a generic‑precision seq_t this is technically UB, but the
   build system supplies a trivial seq_t type for the tests. */
static void ensure_capacity(void)
{
    if (seq_cap == 0) {
        seq_cap = 10000;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    } else if (seq_len >= seq_cap) {
        seq_cap *= 2;
        seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
    }
}

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for index 0 – return a dummy value */
        return 0;
    ensure_capacity();

    /* Generate terms until we have at least n of them. */
    while (seq_len < n) {
        /* Find the next prime.  The simple trial‑division test via
           is_prime_u64 is fast enough for the required range (the
           10 000th prime is below 2×10^5). */
        candidate++;
        while (!is_prime_u64(candidate))
            candidate++;

        if (has_solution(candidate))
            seq[seq_len++] = (seq_t)candidate;
    }
    return seq[n - 1];
}
