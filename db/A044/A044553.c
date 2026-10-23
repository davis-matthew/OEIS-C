/*
 * A044553  Numbers n such that string 4,6 occurs in the base 7 representation
 *          of n but not of n+1.
 * https://oeis.org/A044553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A044553
 * Numbers n such that the base‑7 representation of n contains the digit
 * string "46" but n+1 does not.
 *
 * This code implements a fast incremental search for the nth such number.
 * The sequence has offset 1: a(1) = 34.
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Return true iff n written in base 7 contains the adjacent digits 4,6
 * (with 4 the more significant digit). */
static bool has46(seq_t n)
{
    if (n == 0) return false;
    seq_t prev = n % (seq_t)7;
    n /= 7;
    while (n > 0) {
        seq_t d = n % (seq_t)7;
        if (d == 4 && prev == 6) return true;
        prev = d;
        n /= 7;
    }
    return false;
}

/* Find the first integer greater than start that satisfies the
 * sequence condition. */
static seq_t next_candidate(seq_t start)
{
    seq_t n = start + 1;
    for (;;) {
        if (has46(n) && !has46(n + 1))
            return n;
        n++;
    }
}

/* Dynamic cache of computed terms.  The array is grown with realloc. */
static seq_t *cache       = NULL;
static size_t cache_len   = 0;   /* number of terms stored */
static size_t cache_cap   = 0;   /* capacity of the cache */

/* Ensure that the cache can hold at least 'need' elements. */
static void cache_grow(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < need) new_cap = need;
    seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (newp == NULL) {
        /* In a production setting we would handle out‑of‑memory */
        abort();
    }
    cache = newp;
    cache_cap = new_cap;
}

/* Main function: a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n < 1 */

    size_t want = (size_t)n;         /* assume n fits in size_t */

    if (want <= cache_len)           /* already computed */
        return cache[want - 1];

    /* Compute new terms up to 'want' */
    seq_t last = cache_len ? cache[cache_len - 1] : (seq_t)0;
    for (size_t i = cache_len; i < want; ++i) {
        seq_t cand = next_candidate(last);
        cache_grow(i + 1);
        cache[i] = cand;
        cache_len = i + 1;
        last = cand;
    }
    return cache[want - 1];
}
