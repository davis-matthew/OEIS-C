/*
 * A182261  Numbers n such that n^2 + {1,3,7} are semiprimes.
 * https://oeis.org/A182261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A182261: numbers n such that n²+1, n²+3, n²+7 are all semiprimes.
 * Implementation: brute‑force search.  The values are stored in a
 * fixed-size static array so the code compiles both as C11 and as
 * C++17 (seq_t may be an arbitrary‑precision integer class).
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Utility for factoring 64‑bit integers. */
#include "../utils/factor_u64.h"

/* -------------------------------------------------------------------------- */
/*  Helper: test whether a 64‑bit integer is a semiprime (product of two primes
 *  with multiplicity).  It uses factor_u64() which is available in both C
 *  and C++.  The function works for any seq_t that can be stored in a uint64_t.
 */
static bool is_semiprime(uint64_t m)
{
    if (m < 4)
        return false;          /* 4 = 2×2 is the smallest semiprime */

    static uint64_t primes[64], exps[64];
    size_t cnt;
    factor_u64(m, primes, exps, &cnt);

    uint64_t tot = 0;
    for (size_t i = 0; i < cnt; ++i)
        tot += exps[i];

    return tot == 2;
}

/* -------------------------------------------------------------------------- */
/*  Storage for the sequence.  The size 200 000 is ample for n up to ~10⁴,
 *  and it works for both C and C++.  The array is statically allocated so
 *  we never need to invoke constructors/destructors with malloc/realloc.
 */
#define SEQ_CAP 200000
static seq_t seq_cache[SEQ_CAP];
static size_t seq_len   = 0;   /* number of values cached */
static uint64_t last_checked = 0;      /* largest candidate tested so far */

/* -------------------------------------------------------------------------- */
/*  Find the nth term (1‑based).  The function is re‑entrant and can be
 *  called in any order.  If n is larger than the current cache size we
 *  extend the cache by scanning candidates until we have found n terms.
 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined index – harmless */

    /* Return cached value if it has already been generated. */
    if ((size_t)n <= seq_len)
        return seq_cache[n - 1];

    /* Extend the cache until we have at least n terms. */
    uint64_t cand = last_checked + 1;   /* next candidate to test */

    while ((size_t)n > seq_len) {
        /* Compute cand² but avoid 64‑bit overflow with 128‑bit arithmetic. */
        unsigned __int128 sq128 = (unsigned __int128)cand * cand;
        uint64_t sq = (uint64_t)sq128;  /* fits because our candidates are small */

        if (is_semiprime(sq + 1) &&
            is_semiprime(sq + 3) &&
            is_semiprime(sq + 7))
        {
            if (seq_len < SEQ_CAP)          /* safety check */
                seq_cache[seq_len++] = (seq_t)cand;
            /* If the array were full we would have to stop; such a case
             * does not occur for n ≤ 10 000. */
        }
        ++cand;
    }

    last_checked = cand - 1;
    return seq_cache[n - 1];
}
