/*
 * A095284  Primes in whose binary expansion the number of 1 bits is > 5 +
 *          number of 0 bits.
 * https://oeis.org/A095284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   primes p for which the number of 1‑bits in the binary expansion
 *            of p exceeds 5 plus the number of 0‑bits. a(n) is the n‑th such
 *            prime (a(1)=127). This implementation searches consecutively for
 *            primes, computing the popcount of each until the condition is
 *            satisfied
 * Range:     u8 n <= 5, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"   /* bool is_prime(idx_t n); */

seq_t a(idx_t n)
{
    /* Return 0 for offset < 1 – behaviour here does not matter. */
    if (n == 0) return (seq_t)0;

    /* Static cache for memoisation (thread‑unsafe, as required by the
       specification).  The cache grows as necessary. */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;

    /* The last integer that was examined for primality.  The first
       candidate is 2, so start with 1. */
    static uint64_t last_checked = 1;

    /* Generate until we have at least n terms. */
    while (len < n) {
        /* Find the next prime larger than last_checked.  We use the
           is_prime helper; no external library is required. */
        uint64_t cand = last_checked + 1;
        while (!is_prime(cand)) ++cand;
        last_checked = cand;
        seq_t prime   = (seq_t)cand;

        /* Compute bit length of prime.  The loop runs while prime is
           non‑zero; this works for any seq_t type that supports the
           shift operator. */
        unsigned bitlen = 0;
        seq_t temp = prime;
        while (temp) {
            ++bitlen;
            temp >>= 1;
        }

        /* Count set bits (ones).  We use the classic “iterative
           masking” method, which works for all seq_t types. */
        unsigned ones = 0;
        temp = prime;
        while (temp) {
            ++ones;
            temp &= temp - 1;   /* clear lowest set bit */
        }

        /* The condition: ones > 5 + zeros, where zeros = bitlen – ones. */
        if (ones > 5 + (bitlen - ones)) {
            /* Grow cache if necessary.  Use exponential growth. */
            if (len == cap) {
                cap = (cap == 0) ? 8 : cap * 2;
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            }
            cache[len++] = prime;
        }
    }

    return cache[n - 1];
}
