/*
 * A162406  a(0)=1. a(n) = a(n-1) + (the largest value among
 *          (a(0),a(1),...a(n-1)) that when written in binary occurs as a
 *          substring in the binary representation of n).
 * https://oeis.org/A162406
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = a(n-1) + max{ a(k) | 0 ≤ k < n and the binary
 *            representation of a(k) occurs as a substring of the binary
 *            representation of n }, a(0)=1
 * Range:     u8 n <= 32, u16 n <= 851, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   bitlen_seq_t, seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/bitlen_seq_t.h"
#include "../utils/seq_alloc_array.h"

/* helper: number of bits needed to represent a 64‑bit unsigned integer */
static unsigned bitlen_uint64(uint64_t x)
{
    unsigned len = 0;
    if (x == 0) return 1;
    while (x) {
        ++len;
        x >>= 1;
    }
    return len;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t sz    = 0;

    /* ensure enough space */
    size_t need = (size_t)n + 1;
    if (need > cap) {
        size_t new_cap = cap ? cap * 2 : 1;
        while (new_cap < need) new_cap *= 2;
        seq_t *new_cache = seq_alloc_array(new_cap);
        for (size_t i = 0; i < sz; ++i) new_cache[i] = cache[i];
        cache = new_cache;
        cap   = new_cap;
    }

    if (sz == 0) {
        cache[0] = (seq_t)1;   /* a(0) = 1 */
        sz      = 1;
    }

    /* compute terms up to n */
    for (size_t ii = sz; ii <= (size_t)n; ++ii) {
        if (ii == 0) continue;            /* already set */

        uint64_t n_val = (uint64_t)ii;
        unsigned len_n = bitlen_uint64(n_val);

        seq_t M = (seq_t)0;               /* largest matching previous term */

        /* search previous terms from largest to smallest */
        for (size_t jj = ii; jj-- > 0; ) {
            seq_t val = cache[jj];
            unsigned len_v = bitlen_seq_t(val);
            if (len_v > len_n) continue;  /* cannot be a substring */

            uint64_t v64 = (uint64_t)val;
            uint64_t mask = (len_v == 64) ? ~0ULL
                                          : ((uint64_t)1 << len_v) - 1;
            unsigned max_shift = len_n - len_v;
            for (unsigned shift = 0; shift <= max_shift; ++shift) {
                if (((n_val >> shift) & mask) == v64) {
                    M = val;
                    goto found_match;
                }
            }
        }
found_match:
        cache[ii] = cache[ii - 1] + M;
    }

    sz = n + 1;
    return cache[n];
}
