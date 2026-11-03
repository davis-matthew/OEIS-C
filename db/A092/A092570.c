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
 * Range:     u8 n <= 28, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 73 terms through n = 879 (2026-11-03)
 * Helpers:   bit_length, is_prime_u64, mem
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
#include <stdint.h>
#include "../utils/bit_length.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

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
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
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
    unsigned bits = bit_length(x);
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
