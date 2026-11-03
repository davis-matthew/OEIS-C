/*
 * A043934  Numbers k such that 0 and 3 occur juxtaposed in the base-8
 *          representation of k but not of k+1.
 * https://oeis.org/A043934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Validated: partial, pass, 43 terms through n = 43 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A043934:
   Numbers k such that 0 and 3 occur juxtaposed in the base‑8 representation
   of k but not of k+1.  The function searches successive integers,
   memoizing results so that any order of requests is handled efficiently. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Check whether an octal representation of x contains two adjacent
   digits that are 0 and 3 in either order.  Leading zeros are not present
   in the representation. */
static bool has_adjacent_03_30(seq_t x)
{
    int prev = -1;          /* initialize to an impossible value */
    while (x > (seq_t)0) {
        int cur = (int)(x % (seq_t)8);
        if (prev >= 0 &&
            ((prev == 0 && cur == 3) || (prev == 3 && cur == 0)))
            return true;
        prev = cur;
        x /= (seq_t)8;
    }
    return false;
}

/* Static storage for memoization */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static seq_t next_k = 0;   /* next number to test */

/* Expand cache capacity when needed */
static void cache_grow(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    seq_t *new_buf = mem_resize(seq_t, cache, new_cap);
    cache = new_buf;
    cache_cap = new_cap;
}

/* Compute sequence values up to index n.  The function can be called
   repeatedly in any order; it ensures that cache holds at least n
   elements. */
static void compute_up_to(idx_t n)
{
    while (cache_len < (size_t)n) {
        bool cond_k  = has_adjacent_03_30(next_k);
        bool cond_k1 = has_adjacent_03_30(next_k + (seq_t)1);
        if (cond_k && !cond_k1) {
            if (cache_len == cache_cap)
                cache_grow();
            cache[cache_len++] = next_k;
        }
        next_k++;
    }
}

/* Public API: returns the n-th term of A043934 (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n=0, return 0 */
    compute_up_to(n);
    return cache[n - 1];
}
