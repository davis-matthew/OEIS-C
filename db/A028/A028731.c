/*
 * A028731  Nonsquares mod 18.
 * https://oeis.org/A028731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = the nth positive integer that is not a quadratic *
 *            residue modulo 18. (Equivalently, the n-th integer m such that *
 *            x² ≡ m (mod 18) has no integer solution x.)
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* 1, 4, 7, 9, 10, 13, 16, 0 are the quadratic residues modulo 18.
 * The array below marks true for residues, false for non–residues. */
static const unsigned char is_residue[18] = {
    1,  /* 0 */
    1,  /* 1 */
    0,  /* 2 */
    0,  /* 3 */
    1,  /* 4 */
    0,  /* 5 */
    0,  /* 6 */
    1,  /* 7 */
    0,  /* 8 */
    1,  /* 9 */
    1,  /*10 */
    0,  /*11 */
    0,  /*12 */
    1,  /*13 */
    0,  /*14 */
    0,  /*15 */
    1,  /*16 */
    0   /*17 */
};

/* Static cache of already generated sequence terms. */
static seq_t *cache = NULL;        /* array of terms */
static idx_t cache_len = 0;        /* number of terms stored */
static size_t cache_cap = 0;       /* allocated capacity */
static seq_t next_candidate = 1;   /* next integer to test */

/* Grow the cache when needed. */
static void grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap << 1 : 64;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* Out of memory: terminate the program (builds typically
         * handle this externally). */
        abort();
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the nth term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined for 0, but return 0 safely */

    /* Generate terms up to n. */
    while (cache_len < n) {
        /* Ensure space for new term. */
        if (cache_len >= cache_cap)
            grow_cache();

        /* Check if the current candidate is a non‑residue modulo 18. */
        int mod = (int)(next_candidate % 18);
        if (!is_residue[mod]) {
            cache[cache_len] = next_candidate;
            cache_len++;
        }

        /* Move to the next integer. */
        next_candidate++;
    }

    return cache[n - 1];
}
