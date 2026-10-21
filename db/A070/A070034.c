/*
 * A070034  Numbers n such that n! reduced modulo 2^n is also a power of 2.
 * https://oeis.org/A070034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n such that (n! mod 2^n) is a power of 2
 * Range:     u8 n <= 53, u16 n <= 800, u32 n <= 800, u64 n <= 800,
 *            u128 n <= 800, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static state persistent across calls */
    static seq_t *cache = NULL;       /* stored sequence values */
    static idx_t  cache_len = 0;      /* number of terms stored */
    static idx_t  cache_cap = 0;      /* current allocation size */
    static idx_t  last_k   = 0;       /* largest index processed so far */
    static uint64_t prod   = 1;       /* product of odd parts of 1..last_k (mod 2^64) */

    if (n == 0) return 0;            /* offset starts at 1, no term 0 */

    /* Extend cache until at least n terms are available */
    while (cache_len < n) {
        /* move to next candidate */
        ++last_k;                    /* k = 1,2,3,... */

        /* odd part of k = k with trailing zero bits removed */
        unsigned long long odd_part = (unsigned long long)last_k
                                     >> __builtin_ctzll((unsigned long long)last_k);

        /* accumulate product of odd parts (mod 2^64, natural overflow) */
        prod = prod * odd_part;

        /* v2(k!) = k - popcount(k), so 2^(k - v2) = 2^(popcount(k)) */
        int bits = __builtin_popcountll((unsigned long long)last_k);

        /* check if odd part ≡ 1 (mod 2^bits) */
        bool ok;
        if (bits >= 64) {            /* very rare, but safe for completeness */
            ok = (prod == 1);
        } else {
            uint64_t mask = ((uint64_t)1 << bits) - 1;
            ok = ((prod & mask) == 1);
        }

        if (ok) {
            /* store the term */
            if (cache_len == cache_cap) {
                cache_cap = cache_cap ? cache_cap << 1 : 16;
                cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
            }
            cache[cache_len++] = (seq_t)last_k;
        }
    }

    return cache[n - 1];
}
