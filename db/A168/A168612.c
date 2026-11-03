/*
 * A168612  In binary starting with 1, prepend a 1 and as few 0's as required
 *          such that the new number is relatively prime to all previous in
 *          sequence. Thus binary 1, 11, 111, 10111, 110111, 100110111,
 *          1100110111, 11100110111.
 * https://oeis.org/A168612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 1. For n > 1, let p = a(n-1) and L = bitlen(p). Find the
 *            smallest k ≥ 0 such that x = 2^(L+k) + p is coprime to every
 *            a(i), 1 ≤ i < n. Then a(n) = x
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 15, u64 n <= 28, u128 n <= 40,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 160 (2026-11-03)
 * Helpers:   bit_length, gcd, mem, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses a static cache.  It grows the cache on demand
   and keeps each term for future queries.  Candidate numbers are built
   incrementally by doubling a starting power of two, which avoids
   repeated large allocations.  Gcd calculations are done with the
   provided utilities. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/bit_length.h"
#include "../utils/gcd.h"
#include "../utils/pow2.h"
#include "../utils/mem.h"

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* Static cache of previously computed terms */
    static seq_t *cache = NULL;
    static size_t cap  = 0;   /* allocated capacity of cache   */
    static size_t len  = 0;   /* number of terms stored (a(1)=cache[0]) */

    if (n == 0)   /* undefined, but avoid UB */
        return (seq_t)0;

    /* Build terms up to index n (1‑based) */
    while (len < n) {
        if (len == 0) {               /* first term */
            grow_seq(&cache, &cap, 1);
            cache[0] = (seq_t)1;
            ++len;
            continue;
        }

        /* Prepare to find a(n) from a(n-1) */
        seq_t prev = cache[len - 1];
        unsigned L   = bit_length(prev);   /* bit length of prev */

        seq_t pow   = pow2((idx_t)L);        /* 2^L  */
        seq_t cand;
        bool ok;

        for (;;) {
            cand = pow + prev;               /* candidate = prev + 2^(L+k) */

            /* Quick failure test: gcd(prev, 2^(L+k)) must be 1 */
            if (gcd(prev, pow) != (seq_t)1)
                ok = false;
            else {
                /* Check coprimality against all earlier terms */
                ok = true;
                for (idx_t j = 0; j < len; ++j) {
                    if (gcd(cand, cache[j]) != (seq_t)1) {
                        ok = false;
                        break;
                    }
                }
            }

            if (ok) break;                  /* found the next term */
            /* otherwise try next k: double the power of two */
            pow = mul_small(pow, 2);
        }

        /* Store new term into cache */
        grow_seq(&cache, &cap, len + 1);     /* ensure capacity */
        cache[len] = cand;
        ++len;
    }

    return cache[n - 1];   /* cache[0] holds a(1) */
}
