/*
 * A044464  Numbers k such that string 1,1 occurs in the base 5 representation
 *          of k but not of k+1.
 * https://oeis.org/A044464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by scanning integers and selecting those k whose base‑5
   representation contains the substring “11” while the representation of
   k+1 does not. */
#include <stdbool.h>
#ifdef __cplusplus
# include <vector>
#endif
#include <stdlib.h>

static seq_t next_k = (seq_t)0;          /* next integer to test */

static bool has_pair11(seq_t x) {
    int prev = 0;
    while (x > (seq_t)0) {
        int digit = (int)(x % (seq_t)5);
        if (digit == 1 && prev == 1) return true;
        prev = digit;
        x /= (seq_t)5;
    }
    return false;
}

#ifdef __cplusplus
static std::vector<seq_t> cache;         /* cached results */
#else
static seq_t *cache   = NULL;            /* cached results */
static size_t cache_cap  = 0,            /* capacity of cache   */
              cache_len  = 0;            /* current size of cache */
#endif

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;        /* offset is 1, so 0 is undefined */

#ifdef __cplusplus
    while (cache.size() < (size_t)n) {
        if (has_pair11(next_k) && !has_pair11(next_k + (seq_t)1))
            cache.push_back(next_k);
        next_k++;
    }
    return cache[n - 1];
#else
    while (cache_len < (size_t)n) {
        if (has_pair11(next_k) && !has_pair11(next_k + (seq_t)1)) {
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return (seq_t)0;       /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = next_k;
        }
        next_k++;
    }
    return cache[n - 1];
#endif
}
