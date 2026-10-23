/*
 * A191401  k-almost primes which are digitally balanced in base 2 and have 2k
 *          digits in base 2, for some k >= 1.
 * https://oeis.org/A191401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   sequence A191401 – k‑almost primes that are digitally balanced *
 *            in base 2 and have exactly 2k binary digits. * The term a(n) is
 *            the n‑th integer having * • binary length 2k (k≥1), * •
 *            popcount(k), * • total number of prime factors (with
 *            multiplicity) = k. * A cache is used so that calls with arb...
 * Range:     u8 n <= 17, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   uint64_omega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/uint64_omega.h"

/* --------------------------------------------------------------------- */
/*  Helpers for the bit and prime‑factor calculations                     */
/* --------------------------------------------------------------------- */

/* Count bits in an unsigned integer of type seq_t. */
static unsigned bitlen_seq_t(seq_t x)
{
    unsigned len = 0;
    while (x) {
        ++len;
        x >>= 1;
    }
    return len;
}

/* Count ones (popcount) in an unsigned integer of type seq_t. */
static unsigned popcount_seq_t(seq_t x)
{
    unsigned cnt = 0;
    while (x) {
        cnt += (unsigned)(x & 1);
        x >>= 1;
    }
    return cnt;
}

/* --------------------------------------------------------------------- */
/*  Cache for the discovered sequence elements                           */
/* --------------------------------------------------------------------- */
static seq_t *term_cache     = NULL;
static size_t term_cache_sz  = 0;
static size_t term_cache_cap = 0;
static idx_t  search_start   = 1;   /* next integer to test           */

/* Ensure the cache has room for at least 'need' elements. */
static void ensure_cache(size_t need)
{
    if (need <= term_cache_cap) return;
    size_t new_cap = term_cache_cap ? term_cache_cap << 1 : 64;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(term_cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    term_cache = tmp;
    term_cache_cap = new_cap;
}

/* --------------------------------------------------------------------- */
/*  Main sequence function                                                */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for 0, harmless */

    ensure_cache((size_t)n);

    /* Generate terms until we have at least n of them. */
    while (term_cache_sz < (size_t)n) {
        /* Find next candidate satisfying all conditions. */
        for (;;) {
            ++search_start;
            if (search_start < 2) continue;   /* skip 0 and 1 */

            /* Bit length check. */
            unsigned bitlen = bitlen_seq_t((seq_t)search_start);
            if (bitlen < 2 || (bitlen & 1u)) continue;   /* need even length */

            unsigned k = bitlen >> 1;     /* half the length */

            /* Popcount must equal k. */
            if (popcount_seq_t((seq_t)search_start) != k) continue;

            /* Total prime factor count must equal k. */
            if (uint64_omega((uint64_t)search_start) != k) continue;

            /* Candidate accepted. */
            term_cache[term_cache_sz++] = (seq_t)search_start;
            break;
        }
    }

    return term_cache[(size_t)n - 1];
}
