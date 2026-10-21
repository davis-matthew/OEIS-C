/*
 * A087268  Solutions to gcd(x, pi(x)) = 1, where pi is A000720.
 * https://oeis.org/A087268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 149, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A087268: a(n) is the nth integer x such that gcd(x, pi(x)) = 1,
   where pi(x) is the prime counting function. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/is_prime.h"

/* keep a memoised list of found numbers so that calls in any order are fast */
static seq_t *cache = NULL;          /* array of found numbers */
static size_t cache_len = 0;         /* number of elements stored */
static size_t cache_cap = 0;         /* current allocation size */

/* state for generating new numbers */
static idx_t next_x = 1;             /* next integer to test (starting at 1) */
static seq_t pi_cur = 0;             /* pi(next_x-1) immediately before testing next_x */

/* helper to grow the cache if necessary */
static void ensure_cache_cap(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 8 : *cap * 2;
    while (new_cap < needed) new_cap <<= 1;
    *ptr = realloc(*ptr, new_cap * elem_size);
    *cap = new_cap;
}

/* return the nth term of OEIS A087268 (n >= 1) */
seq_t a(idx_t n)
{
    /* generate terms until we have at least n of them */
    while (cache_len < n) {
        /* test current number next_x */
        bool is_p = is_prime(next_x);        /* true iff next_x is prime */
        if (is_p) pi_cur++;                  /* pi(next_x) = pi(next_x-1)+1 if prime */

        if (gcd((seq_t)next_x, pi_cur) == 1) { /* satisfies the condition */
            ensure_cache_cap(cache_len + 1, &cache_cap, (void**)&cache, sizeof(seq_t));
            cache[cache_len++] = next_x;
        }

        next_x++;                            /* move to next integer */
    }

    /* return the (n-1)-th stored value (0‑based index) */
    return cache[n - 1];
}
