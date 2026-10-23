/*
 * A044182  Numbers n such that string 6,2 occurs in the base 7 representation
 *          of n but not of n-1.
 * https://oeis.org/A044182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A044182.
   A number appears if its base‑7 representation contains the
   digit pair 6,2, but the preceding integer (n‑1) does not. */

#include <stdlib.h>
#include <stdbool.h>

static bool has62(seq_t v)
{
    /* return true if base‑7 digits contain the adjacent pair 6,2 */
    if (v == (seq_t)0) return false;
    seq_t prev = (seq_t)-1;          /* sentinel larger than 6 */
    while (v != (seq_t)0) {
        seq_t curr = v % (seq_t)7;
        v     /= (seq_t)7;
        if (prev == (seq_t)2 && curr == (seq_t)6) return true;
        prev = curr;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;

    static seq_t *cache = NULL;      /* cached terms */
    static size_t cache_len = 0;     /* number of cached terms */
    static size_t cache_cap = 0;     /* capacity of cache array */
    static seq_t upper_limit = 0;    /* last number examined */

    while (cache_len < (size_t)n) {
        upper_limit++;  /* examine next integer */

        if (has62(upper_limit) && !has62(upper_limit - (seq_t)1)) {
            /* store this term */
            if (cache_len == cache_cap) {
                size_t newcap = cache_cap ? cache_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                if (tmp == NULL) abort();   /* allocation failure */
                cache = tmp;
                cache_cap = newcap;
            }
            cache[cache_len++] = upper_limit;
        }
    }

    return cache[(size_t)n - 1];
}
