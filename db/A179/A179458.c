/*
 * A179458  Numbers k such that ((2^(2k) - 1) mod 2k) - (2^(2k-1) mod 2k) = 1.
 * https://oeis.org/A179458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 2267, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A179458
 *
 * A number k (k ≥ 1) is in the sequence iff
 *   ((2^(2k) – 1) mod 2k) – (2^(2k‑1) mod 2k) = 1
 *
 * The implementation incrementally builds the list of sequence terms
 * until the n‑th term is available.  Subsequent calls reuse the cached
 * list.  All arithmetic is performed with the build‑specific type
 * seq_t, and exponentiation uses the supplied mod_pow utility.
 *
 * The function works for any build (8, 16, 32, 64, 128‑bit or
 * arbitrary‑precision) as long as the user is only asked for terms
 * that fit into seq_t.  It runs in O(n log n) time with a small
 * constant: for each candidate k we perform two modular exponentiations
 * with exponent size 2k (≤ 20000 for n ≤ 10000).  Memory usage is
 * O(n) for storing the cached terms.
 */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* static cache of already discovered sequence terms */
    static seq_t *cache = NULL;   /* pointer to allocated array */
    static size_t  cache_cap = 0; /* current capacity of cache */
    static size_t  cache_len = 0; /* number of terms stored */
    static idx_t  next_k = 1;     /* first k to test next */

    /* continue generating terms until we have at least n values */
    while (cache_len < n)
    {
        /* modulo value m = 2 * k */
        idx_t    two_k_idx = next_k << 1;           /* fit in idx_t */
        seq_t    m = (seq_t)two_k_idx;              /* cast to seq_t */

        /* modular powers 2^(2k-1) mod 2k and 2^(2k) mod 2k */
        seq_t a1 = mod_pow((seq_t)2, (uint64_t)(two_k_idx - 1), m);
        seq_t a2 = mod_pow((seq_t)2, (uint64_t)(two_k_idx),     m);

        /* compute ((a2 - 1) mod m) */
        seq_t left;
        if (a2 == 0)
            left = m - 1;           /* -1 mod m */
        else
            left = a2 - 1;          /* a2 - 1, guaranteed < m */

        /* condition: left == a1 + 1 */
        if (left == a1 + 1)
        {
            /* append next_k to cache */
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
                seq_t *new_mem = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_mem) { /* allocation failed – abort */
                    exit(1);
                }
                cache = new_mem;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_k;
        }
        next_k++;  /* test next k in following iteration */
    }

    return cache[n - 1];
}
