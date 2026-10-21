/*
 * A095320  Primes in whose binary expansion the number of 1-bits is > number
 *          of 0-bits minus 3.
 * https://oeis.org/A095320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th prime p such that popcount(p) > popcount(~p) -
 *            3. The sequence is computed by scanning successive primes and
 *            testing the pop‑count condition. The test uses only integer
 *            arithmetic and bitwise operations
 * Range:     u8 n <= 54, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_small.h"   /* provides bool is_prime_small(seq_t) */

/* Helper to count set bits of a seq_t value. */
static inline unsigned popcount_seq(seq_t x)
{
    unsigned c = 0;
    while (x) {
        c += (unsigned)(x & 1);
        x >>= 1;
    }
    return c;
}

/* Helper to compute the bit length of a seq_t value. */
static inline unsigned bitlen_seq(seq_t x)
{
    unsigned l = 0;
    while (x) {
        x >>= 1;
        l++;
    }
    return l ? l : 1;   /* bit length of 0 is defined as 1 (not used here) */
}

/* Cache of computed terms. */
static seq_t *cache     = NULL;   /* array of qualifying primes      */
static size_t cap       = 0;       /* capacity of the array          */
static size_t count     = 0;       /* number of stored terms          */
static seq_t candidate  = 0;       /* current candidate prime        */

/* Extend the cache until it contains at least n terms. */
static void ensure_cache(idx_t n)
{
    while (count < n) {
        /* Advance candidate to the next integer.  Keep only odd numbers
           after we hit 2 to skip even composites. */
        if (candidate == 0)
            candidate = 2;
        else
            candidate++;

        /* Test primality.  If not prime, skip to the next iteration. */
        if (!is_prime_small(candidate))
            continue;

        /* Compute popcount and bit length. */
        unsigned pop  = popcount_seq(candidate);
        unsigned bits = bitlen_seq(candidate);

        /* Number of zero bits in the binary representation. */
        unsigned zeros = bits - pop;

        /* Compute the threshold:  zeros-3, treating negative as 0. */
        unsigned threshold = zeros > 3 ? zeros - 3 : 0;

        /* Check the sequence condition: pop > zeros-3. */
        if (pop > threshold) {
            /* Grow the cache array if necessary. */
            if (count == cap) {
                size_t newcap = (cap == 0) ? 16 : cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                if (!tmp)
                    abort();   /* allocation failed */
                cache = tmp;
                cap   = newcap;
            }
            /* Store the qualifying prime. */
            cache[count++] = candidate;
        }
    }
}

/* Sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for n<1, return 0 for safety */
    ensure_cache(n);
    return cache[n - 1];
}
