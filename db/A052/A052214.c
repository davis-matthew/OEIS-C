/*
 * A052214  Numbers n with prime signature(n) = prime signature(n+1) = prime
 *          signature(n+2).
 * https://oeis.org/A052214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 603, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_capacity, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers n such that prime signature(n)=prime signature(n+1)=prime signature(n+2) */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/ensure_capacity.h"

/* Compute the sorted list of exponents in the prime factorisation of x.
   The list is stored in e[0..len-1] and its length is written to *plen. */
static void signature_exponents(uint64_t x, uint8_t e[64], size_t *plen)
{
    static const size_t MAX_FACTORS = 64;
    uint64_t primes[MAX_FACTORS];
    uint64_t exps[MAX_FACTORS];
    size_t cnt;

    if (x <= 1) {
        *plen = 0;
        return;
    }

    factor_u64(x, primes, exps, &cnt);

    /* copy exponents and sort them */
    for (size_t i = 0; i < cnt; ++i)
        e[i] = (uint8_t)exps[i];

    /* simple insertion sort – cnt is tiny */
    for (size_t i = 1; i < cnt; ++i) {
        uint8_t key = e[i];
        size_t j = i;
        while (j > 0 && e[j - 1] > key) {
            e[j] = e[j - 1];
            --j;
        }
        e[j] = key;
    }
    *plen = cnt;
}

/* Return true if the prime signatures of a,b,c are identical. */
static bool same_signature_three(uint64_t a, uint64_t b, uint64_t c)
{
    uint8_t sa[64], sb[64], sc[64];
    size_t la, lb, lc;

    signature_exponents(a, sa, &la);
    signature_exponents(b, sb, &lb);
    signature_exponents(c, sc, &lc);

    if (la != lb || lb != lc)
        return false;

    for (size_t i = 0; i < la; ++i)
        if (sa[i] != sb[i] || sb[i] != sc[i])
            return false;
    return true;
}

/* Global cache of computed sequence values. */
static seq_t *seq_cache       = NULL;
static size_t  seq_cache_len  = 0;
static size_t  seq_cache_cap  = 0;
static idx_t   next_candidate = 1;

/* Return the n-th term of the sequence (offset 1). */
seq_t a(idx_t n)
{
    /* Generate sequence terms until we have at least n values. */
    while (seq_cache_len < (size_t)n) {
        uint64_t i = (uint64_t)next_candidate;

        if (same_signature_three(i, i + 1, i + 2)) {
            /* Grow the cache if necessary. */
            if (seq_cache_len == seq_cache_cap) {
                size_t new_cap = seq_cache_cap ? seq_cache_cap * 2 : 16;
                ensure_capacity(seq_cache_len + 1, &seq_cache_cap,
                                (void **)&seq_cache, sizeof(seq_t));
            }
            seq_cache[seq_cache_len++] = (seq_t)i;
        }
        ++next_candidate;
    }

    return seq_cache[n - 1];
}
