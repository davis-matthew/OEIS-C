/*
 * A092570  Primes p which become a prime q under transformation of inner bits
 *          of binary representation in A092569. In binary representation of
 *          p, transformation of inner bits, 1 <-> 0, gives binary
 *          representation of q.
 * https://oeis.org/A092570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 999, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   bitlen_seq_t, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A092570: primes p such that flipping every inner bit of their
   binary representation yields another prime q.  The sequence is generated
   by testing primes incrementally and keeping the ones that satisfy
   the condition. */
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include "../utils/is_prime_big.h"
#include "../utils/bitlen_seq_t.h"

/* Static cache of computed terms */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_count = 0;

/* Next candidate prime to examine (persistent across calls) */
static seq_t next_candidate = 2;

/* Grow the cache to accommodate more terms */
static void grow_cache(void)
{
    size_t new_cap = (cache_size == 0) ? 4 : cache_size * 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    assert(tmp != NULL);
    cache = tmp;
    cache_size = new_cap;
}

/* Append a value to the cache, growing if necessary */
static void append_value(seq_t val)
{
    if (cache_count == cache_size)
        grow_cache();
    cache[cache_count++] = val;
}

/* Compute the mask that flips all inner bits of x */
static inline seq_t inner_flip_mask(seq_t x)
{
    unsigned bits = bitlen_seq_t(x);
    if (bits <= 2)
        return (seq_t)0;
    seq_t mask = 0;
    unsigned inner_bits = bits - 2;
    for (unsigned i = 0; i < inner_bits; ++i)
    {
        mask = (mask << 1) | ((seq_t)1);
    }
    mask <<= 1;
    return mask;
}

/* Return the nth term (n >= 1) of the sequence */
seq_t a(idx_t n)
{
    assert(n > 0);

    /* Generate terms until we have at least n */
    while (cache_count < (size_t)n)
    {
        seq_t cand = next_candidate;

        if (is_prime_big(cand))
        {
            seq_t mask = inner_flip_mask(cand);
            seq_t q = cand ^ mask;
            if (is_prime_big(q))
            {
                append_value(cand);
            }
        }

        /* Prepare for next candidate */
        next_candidate = cand + 1;
    }

    return cache[(size_t)(n - 1)];
}
