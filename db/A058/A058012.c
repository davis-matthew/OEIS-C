/*
 * A058012  Consider the sequence {b(m)} of composite numbers (excluding 1);
 *          sequence gives values of b(m) where gcd(m, b(m)) increases.
 * https://oeis.org/A058012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th composite number b(m) for which g(m) = gcd(m,
 *            b(m)) strictly increases. (b(1)=4,b(2)=6,b(3)=8,…)
 * Range:     u8 n <= 14, u16 n <= 36, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Validated: partial, pass, 43 terms through n = 43 (2026-11-03)
 * Helpers:   gcd, is_prime_seq, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/is_prime_seq.h"
#include "../utils/mem.h"

static inline bool isComposite(seq_t n) { return n > 1 && !is_prime_seq(n); }

/* Return the n-th term of A058012.  Indexing starts at 1. */
seq_t a(idx_t n)
{
    /* Cache of already computed terms; cache[0] = a(1). */
    static seq_t *cache   = NULL;
    static size_t  cache_len = 0;
    static size_t  cache_cap = 0;

    /* Incremental state for the scan.  These survive between calls. */
    static seq_t   candidate  = 4;         /* current integer being examined */
    static idx_t   comp_index = 0;         /* index of current composite in b(m) */
    static seq_t   prev_gcd   = 0;         /* gcd of previous recorded term */

    /* Return cached value if already known. */
    if (n <= cache_len)
        return cache[n - 1];

    /* Grow cache if necessary. */
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 1;
        /* cast required for C++ compatibility */
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp)    /* allocation failed */
            return (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate terms until we reach index n. */
    while (cache_len < n) {
        /* Advance to the next composite number. */
        while (!isComposite(candidate))
            ++candidate;

        ++comp_index;                                   /* b(comp_index) = candidate */
        seq_t g = gcd((seq_t)comp_index, candidate);    /* g = gcd(m, b(m)) */

        if (g > prev_gcd) {
            prev_gcd = g;
            cache[cache_len++] = candidate;
        }

        ++candidate; /* next integer to test */
    }

    return cache[n - 1];
}
