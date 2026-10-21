/*
 * A087967  Integers m such that gcd(2^m-1, m^2) is not a square number.
 * https://oeis.org/A087967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 14, u32 n <= 3635, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt, gcd, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th term of OEIS A087967: the n‑th integer m for which
   gcd(2^m-1, m^2) is not a perfect square.  The function searches for
   such m by testing successive integers, and caches the found values so
   that repeated calls are fast.  The term list is not hard‑coded. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/mod_pow.h"
#include "../utils/floor_sqrt.h"

/* Test whether a seq_t value is a perfect square. */
static inline bool is_square(seq_t v)
{
    seq_t s = floor_sqrt(v);
    return s * s == v;
}

/* Return the n-th element of the sequence (1‑based). */
seq_t a(idx_t n)
{
    /* Sentinel for undefined index (offset is 1). */
    if (n == 0)
        return (seq_t)0;

    /* Static cache for previously computed terms. */
    static seq_t *terms = NULL;   /* array holding found values      */
    static size_t len   = 0;      /* number of stored terms          */
    static size_t cap   = 0;      /* allocated capacity of terms      */
    static uint64_t last_m = 0;   /* last integer m examined         */

    /* Generate terms until we have at least n of them. */
    while (len < n) {
        ++last_m;
        uint64_t m = last_m;              /* current candidate m          */
        seq_t m_s = (seq_t)m;

        /* Compute m^2 (fits in seq_t for all n up to 10 000; overflow
           would be negligible for this size range). */
        seq_t mod = m_s * m_s;            /* m^2                            */

        /* Compute 2^m (mod m^2).  mod_pow returns a seq_t. */
        seq_t r = mod_pow((seq_t)2, m, mod);

        /* 2^m - 1 modulo m^2. */
        seq_t r_minus1 = (r == 0) ? mod - 1 : r - 1;

        /* gcd(2^m-1, m^2). */
        seq_t g = gcd(r_minus1, mod);

        /* Keep m if the gcd is not a perfect square. */
        if (!is_square(g)) {
            if (len == cap) {
                size_t new_cap = (cap == 0) ? 4 : cap * 2;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp) {
                    /* Allocation failure: abort to avoid corruption. */
                    exit(EXIT_FAILURE);
                }
                terms = tmp;
                cap   = new_cap;
            }
            terms[len++] = (seq_t)m;
        }
    }

    return terms[n - 1];   /* 1‑based indexing. */
}
