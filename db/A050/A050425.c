/*
 * A050425  Numbers for which in base 2 the least number of digits that can be
 *          removed to leave a base 2 palindromic number (beginning with 1) is
 *          1.
 * https://oeis.org/A050425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A050425 by brute‑force search and caching. */
/*  A(n) is the n‑th positive integer whose binary representation
    can become a palindrome (starting with 1) by removing exactly
    one digit, and is not itself a palindrome.  */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/bitlen_seq_t.h"

/* Cache for already found terms. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* Return true iff x is not a binary palindrome but can become one
   by removing exactly one bit, with the resulting palindrome still
   starting with 1. */
static bool can_become_pal_one_removal(seq_t x)
{
    unsigned len = bitlen_seq_t(x);
    if (len <= 1)
        return false;                         /* single bit cannot be modified */

    unsigned l = 0, r = len - 1;

    while (l < r) {
        unsigned bl = (unsigned)((x >> l) & 1);
        unsigned br = (unsigned)((x >> r) & 1);

        if (bl == br) {                      /* match, shrink window */
            ++l;
            --r;
            continue;
        }

        /* mismatch found at positions l and r */
        bool ok = false;

        /* try deleting the left bit at position l */
        {
            unsigned i = l + 1, j = r;
            while (i < j) {
                unsigned b1 = (unsigned)((x >> i) & 1);
                unsigned b2 = (unsigned)((x >> j) & 1);
                if (b1 != b2)
                    break;
                ++i;
                --j;
            }
            if (i >= j)                       /* palindrome after removal */
                ok = true;
        }

        if (!ok) {
            /* try deleting the right bit at position r
               unless removing it would make the new MSB 0 */
            if (!(r == len - 1 && ((x >> (len - 2)) & 1) == 0)) {
                unsigned i = l, j = r - 1;
                while (i < j) {
                    unsigned b1 = (unsigned)((x >> i) & 1);
                    unsigned b2 = (unsigned)((x >> j) & 1);
                    if (b1 != b2)
                        break;
                    ++i;
                    --j;
                }
                if (i >= j)
                    ok = true;
            }
        }

        return ok;                            /* either way enough for one bit removal */
    }

    /* The original number was already a palindrome, so minimal removal is 0. */
    return false;
}

/* Expand the cache until it contains at least n terms. */
static void ensure_cache(idx_t n)
{
    static seq_t last_tested = 0;                /* last tested candidate */

    while (cache_len < n) {
        /* Test next candidate */
        seq_t candidate = last_tested + 1;
        last_tested = candidate;

        if (can_become_pal_one_removal(candidate)) {
            /* Grow cache if necessary */
            if (cache_len + 1 > cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = candidate;
        }
    }
}

/* Public function required by the exercise. */
seq_t a(idx_t n)
{
    if (n == 0)                           /* safety for offset 1 */
        return 0;
    ensure_cache(n);
    return cache[n - 1];
}
