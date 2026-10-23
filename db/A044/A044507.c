/*
 * A044507  Numbers n such that string 4,0 occurs in the base 6 representation
 *          of n but not of n+1.
 * https://oeis.org/A044507
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A044507 – numbers n such that the base‑6 representation of n contains
   the substring "40" but the base‑6 representation of n+1 does not.
   Computed by brute‑force search. */

#include <stdlib.h>
#include <stdbool.h>

/* Return true if the base‑6 representation of x contains the substring "40". */
static bool has_40(seq_t x)
{
    if (x == (seq_t)0)
        return false;

    /* Store the base‑6 digits (least significant first). */
    int digits[256];
    int len = 0;
    seq_t tmp = x;
    while (tmp > (seq_t)0) {
        digits[len++] = (int)(tmp % (seq_t)6);
        tmp /= (seq_t)6;
    }

    /* Scan from most significant to least significant digit to find "40". */
    for (int i = len - 1; i > 0; --i) {
        if (digits[i] == 4 && digits[i - 1] == 0)
            return true;
    }
    return false;
}

/* Cached sequence values found so far. */
static seq_t  *cache   = NULL;   /* array of found terms */
static size_t cache_sz  = 0;     /* number of terms stored */
static size_t cache_cap = 0;     /* allocated capacity */

/* Next candidate to test. */
static seq_t next_candidate = 1;

/* Return the n-th term (n ≥ 1).  Undefined for n = 0 but returns 0 safely. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* safety for calls below the offset */

    /* Initialise cache lazily. */
    if (!cache) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            exit(1);   /* allocation failed */
    }

    /* Extend the cache until it contains at least n terms. */
    while (cache_sz < (size_t)n) {
        if (has_40(next_candidate) && !has_40(next_candidate + (seq_t)1)) {
            /* Append the new term. */
            if (cache_sz >= cache_cap) {
                cache_cap = cache_cap ? 2 * cache_cap : 16;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp)
                    exit(1);
                cache = tmp;
            }
            cache[cache_sz++] = next_candidate;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
