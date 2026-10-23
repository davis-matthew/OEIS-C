/*
 * A044680  Numbers n such that string 5,3 occurs in the base 9 representation
 *          of n but not of n+1.
 * https://oeis.org/A044680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers n such that the base‑9 representation of n contains the
 *            substring "53" but the base‑9 representation of n+1 does not.
 *            This implementation constructs the sequence on demand and
 *            memoises already found values
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;           /* memoised sequence values */
static size_t cache_capacity = 0;     /* allocated array size   */
static size_t cache_len = 0;          /* number of elements stored */
static seq_t last_checked = 0;        /* last number examined */

/* Return true if base‑9 representation of x contains the pattern 53
   (digit 5 followed by digit 3) in the usual left‑to‑right order. */
static bool has53(seq_t x)
{
    int prev = -1;                    /* digit one step to the left   */
    while (x > (seq_t)0) {
        int d = (int)(x % (seq_t)9);
        if (prev == 3 && d == 5) {
            return true;
        }
        prev = d;
        x /= (seq_t)9;
    }
    return false;
}

/* Ensure the cache has room for at least 'need' more elements. */
static void ensure_cache_capacity(size_t need)
{
    if (cache_len + need <= cache_capacity)
        return;
    size_t new_cap = cache_capacity ? cache_capacity * 2 : 8;
    while (new_cap < cache_len + need)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* In a real system you might handle allocation failure.
           Here we simply abort, which is acceptable for a control
           function that is not called in production. */
        abort();
    }
    cache = tmp;
    cache_capacity = new_cap;
}

/* Return the nth term for the OEIS sequence A044680 (1‑based). */
seq_t a(idx_t n)
{
    /* Construct the sequence until we have at least n terms. */
    while (cache_len < (size_t)n) {
        last_checked = last_checked + (seq_t)1;      /* next candidate */
        seq_t cand = last_checked;
        seq_t next = cand + (seq_t)1;
        if (has53(cand) && !has53(next)) {
            ensure_cache_capacity(1);
            cache[cache_len++] = cand;
        }
    }
    return cache[n - (idx_t)1];
}
