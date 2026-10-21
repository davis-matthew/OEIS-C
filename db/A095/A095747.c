/*
 * A095747  Maximally asymmetric odd primes in binary.
 * https://oeis.org/A095747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th maximally asymmetric odd prime in binary (primes
 *            whose binary representation has 1 at both ends and for every
 *            interior mirrored pair the bits differ)
 * Range:     u8 n <= 20, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   bitlen_seq_t, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/bitlen_seq_t.h"

/* static cache of already found terms */
#define CACHE_MAX 20000
static seq_t cache[CACHE_MAX];
static idx_t cache_len = 0;
static idx_t next_candidate = 3;   /* next odd number to test */

/* Test whether p satisfies the maximal asymmetry condition. */
static bool is_asymmetric(seq_t p)
{
    unsigned len = bitlen_seq_t(p);
    /* All odd primes have 1 at both ends; the outer pair is allowed to be equal.
       For every other mirrored pair the bits must differ. */
    for (unsigned i = 1; i < len - 1 - i; ++i) {
        seq_t left  = (p >> i) & 1;
        seq_t right = (p >> (len - 1 - i)) & 1;
        if (left == right)
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Build up the cache until it contains at least n terms */
    while (cache_len < n) {
        seq_t cand = (seq_t)next_candidate;
        if (is_prime_big(cand) && is_asymmetric(cand)) {
            if (cache_len < CACHE_MAX)
                cache[cache_len++] = cand;
        }
        /* advance to next odd candidate */
        next_candidate += 2;
    }
    return cache[n - 1];
}
