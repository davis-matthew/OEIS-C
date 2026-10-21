/*
 * A071687  Non-palindromic numbers such that either x=q1.Rev[x] or
 *          Rev[x]=q2.x, where R[x]=A004086[x] and q1 or q2 are integers not
 *          divisible by 10.
 * https://oeis.org/A071687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 16, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   rev_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A071687 – non‑palindromic numbers x such that
 * x = q·rev(x) or rev(x) = q·x, where q>1 and q%10≠0. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/rev_decimal.h"

/* Cached sequence terms.  */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* already generated terms */
static size_t cache_cap = 0;   /* allocated capacity   */
static seq_t next_candidate = 1;   /* next number to test */

/* Ensure enough capacity for one more term.  */
static void grow_cache(void)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* unrecoverable allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Store a found term.  */
static void push_term(seq_t val)
{
    grow_cache();
    cache[cache_len++] = val;
}

/* Build the cache up to at least n terms.  */
static void build_up_to(idx_t n)
{
    while (cache_len < n) {
        seq_t rev = rev_decimal(next_candidate);
        if (next_candidate != rev) {
            if (next_candidate % rev == 0) {
                seq_t q = next_candidate / rev;
                if (q > 1 && (q % 10) != 0) push_term(next_candidate);
            } else if (rev % next_candidate == 0) {
                seq_t q = rev / next_candidate;
                if (q > 1 && (q % 10) != 0) push_term(next_candidate);
            }
        }
        ++next_candidate;
    }
}

/* Return a(n), 1‑based.  */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* 0 is undefined; return safe value */
    build_up_to(n);
    return cache[n - 1];
}
