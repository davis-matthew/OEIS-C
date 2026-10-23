/*
 * A044749  Numbers n such that string 3,6 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for integers n such that the decimal string "36" occurs in n but not in n+1. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Return true if the base‑10 representation of x contains the substring "36". */
static bool has36(seq_t x)
{
    if (x < ((seq_t)10)) return false;
    seq_t prev = x % ((seq_t)10);
    x /= ((seq_t)10);
    while (x > ((seq_t)0)) {
        seq_t cur = x % ((seq_t)10);
        if (cur == ((seq_t)3) && prev == ((seq_t)6)) return true;
        prev = cur;
        x /= ((seq_t)10);
    }
    return false;
}

static seq_t *cache = NULL;     /* stored sequence terms */
static idx_t cache_len = 0;     /* number of stored terms */
static idx_t cache_cap = 0;     /* allocated capacity for cache */
static idx_t next = 0;          /* next integer to test */

/* Grow the cache array to hold at least 'needed' terms. */
static void grow_cache(idx_t needed)
{
    if (needed <= cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

/* External linkage as required. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for offset less than 1 */
    while (cache_len < n) {
        /* Search for the next integer satisfying the condition. */
        while (1) {
            seq_t cand = (seq_t)next;
            if (has36(cand) && !has36((seq_t)(next + 1))) {
                grow_cache(cache_len + 1);
                cache[cache_len++] = cand;
                next = next + 1;        /* next integer to test next time */
                break;
            }
            next++;                    /* skip this integer */
        }
    }
    return cache[n - 1];
}
