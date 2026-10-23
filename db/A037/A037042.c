/*
 * A037042  2-White numbers (take power 2, sum 2-digit groups to get same
 *          number).
 * https://oeis.org/A037042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  full, nonn, fini, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 2‑White numbers (Kaprekar numbers).
    A(1)=0, A(2)=1, A(3)=45, A(4)=55, A(5)=99, …
    The function enumerates integers starting from 0 and keeps those
    whose square can be split into groups of two decimal digits (from
    the least significant side).  The sum of the groups must equal the
    original integer.  Results are cached so that any call order works
    and repeated calls are efficient. */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

#if defined(__SIZEOF_INT128__)
typedef unsigned __int128 u128;
#else
typedef unsigned long long u128;
#endif

/*  Returns the n‑th 2‑White number, where a(1) is the first term (0). */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /*  stored 2‑White numbers          */
    static size_t cache_len = 0;  /*  number of entries in cache     */
    static size_t cache_cap = 0;  /*  allocated capacity              */

    /* If we already have the requested term, return it immediately. */
    if (cache_len >= (size_t)n)
        return cache[(size_t)n - 1];

    /* Grow the cache array if needed.  Use a doubling strategy. */
    if (cache_len + 1 > cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;   /*  out of memory; unlikely in test suite */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Find 2‑White numbers until we have at least n terms. */
    seq_t cand = (cache_len == 0) ? (seq_t)0
                                  : (cache[cache_len - 1] + (seq_t)1);

    while (cache_len < (size_t)n) {
        /* Square the candidate using a wider type to avoid overflow. */
        u128 sq = (u128)cand * (u128)cand;

        /* Sum 2‑digit groups starting from the least significant side. */
        seq_t sum = 0;
        while (sq > (u128)0) {
            seq_t part = (seq_t)(sq % (u128)100);
            sum += part;
            sq /= (u128)100;
        }

        if ((seq_t)sum == cand) {    /*  cand is a 2‑White number  */
            cache[cache_len++] = cand;
            if (cache_len == (size_t)n)
                return cand;
        }
        cand++;
    }

    /* Finally, return the requested term. */
    return cache[(size_t)n - 1];
}
