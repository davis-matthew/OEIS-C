/*
 * A091819  Numbers k that have, in binary, the same number of ones as are in
 *          k^2 and which have no nonincreasing binary representation.
 * https://oeis.org/A091819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 8, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   popcount_generic
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: sequential search for integers k such that
   popcount(k) = popcount(k^2) and the binary representation of k
   contains at least one '01' pattern (i.e., it is NOT non‑increasing). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/popcount_generic.h"

/* ------------------------------------------------------------------- */
/* Helper: popcount for generic seq_t                                         */

/* ------------------------------------------------------------------- */
/* Helper: position of most significant bit (index starting from 0)      */
/* ------------------------------------------------------------------- */
static idx_t msb_pos(seq_t x)
{
    idx_t pos = 0;
    seq_t t = x;
    while (t >>= 1)
        ++pos;
    return pos;
}

/* ------------------------------------------------------------------- */
/* Helper: check if binary representation contains a '01' pattern      */
/* ------------------------------------------------------------------- */
static bool has_increasing(seq_t x)
{
    if (x == 0)
        return false;   /* no bits, no pattern */

    idx_t msb = msb_pos(x);
    seq_t prev = (x >> msb) & 1;

    for (idx_t i = msb; i > 0; --i)
    {
        seq_t cur = (x >> (i - 1)) & 1;
        if (prev == 0 && cur == 1)
            return true;
        prev = cur;
    }
    return false;
}

/* ------------------------------------------------------------------- */
/* Helper: check whether a candidate satisfies the sequence property  */
/* ------------------------------------------------------------------- */
static bool is_good(seq_t k)
{
    seq_t pk     = popcount_generic(k);
    seq_t k2     = k * k;
    seq_t pk2    = popcount_generic(k2);

    if (pk != pk2)
        return false;
    if (!has_increasing(k))
        return false;
    return true;
}

/* ------------------------------------------------------------------- */
/* Main sequence function                                               */
/* ------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Simple cache for computed terms */
    static seq_t *cache   = NULL;
    static size_t   cap   = 0;
    static size_t   len   = 0;
    static seq_t   next_k = 0;    /* last tested candidate */

    if (n == 0)
        return 0;                /* undefined for n < 1 */

    /* Allocate initial cache */
    if (cache == NULL)
    {
        cap   = 64;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        len   = 0;
        next_k = 0;
    }

    /* Extend cache until it contains the required term */
    while (len < n)
    {
        /* Find next good candidate */
        do
        {
            ++next_k;
        } while (!is_good(next_k));

        /* Store it */
        if (len >= cap)
        {
            cap *= 2;
            cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        }
        cache[len++] = next_k;
    }

    return cache[n - 1];
}
