/*
 * A028810  Nonsquares mod 97.
 * https://oeis.org/A028810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Computation: a(n) is the nth positive integer that is a quadratic
 * nonresidue modulo 97.  An integer k is a quadratic residue mod 97
 * iff k ≡ x² (mod 97) for some integer x.  We precompute a table of
 * residues for 0…96 and then scan successive integers, collecting
 * those whose residue is false.
 */

#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Table of residues modulo 97.  0 is treated as a residue to
     * simplify the test for k = 97, 194, … which are not included
     * in the sequence. */
    static bool residues[97];
    static bool residues_initialized = false;

    /* Cache of already computed a(k) values. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* Number of terms stored. */
    static size_t cache_cap  = 0;   /* Capacity of cache array. */

    /* Next integer to test. */
    static seq_t next_candidate = 1;

    /* One‑time initialization of the residue table. */
    if (!residues_initialized) {
        for (int i = 0; i < 97; ++i)
            residues[i] = false;
        residues[0] = true;                 /* 0 is a quadratic residue. */
        for (int i = 1; i <= 48; ++i) {     /* 48 non‑zero squares modulo 97. */
            int r = (i * i) % 97;
            residues[r] = true;
        }
        residues_initialized = true;
    }

    /* Grow cache until it holds at least n values. */
    if (n > cache_size) {
        /* Allocate or grow the cache if necessary. */
        if (n > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 128;
            while (new_cap < n) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();              /* Allocation failed. */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Generate new terms. */
        for (; cache_size < n; ++next_candidate) {
            /* Modulo result fits in an unsigned int. */
            unsigned idx = (unsigned)(next_candidate % (seq_t)97);
            if (!residues[idx]) {
                cache[cache_size++] = next_candidate;
            }
        }
    }

    return cache[n - 1];
}
