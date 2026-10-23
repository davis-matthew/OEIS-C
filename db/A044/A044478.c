/*
 * A044478  Numbers n such that string 4,0 occurs in the base 5 representation
 *          of n but not of n+1.
 * https://oeis.org/A044478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of A044478:
   numbers n such that the base‑5 representation of n contains the
   consecutive digits 4,0 but the base‑5 representation of n+1 does
   not.  The sequence is indexed from 1. */

#include <stdlib.h>
#include <stdbool.h>

#if defined(__cplusplus)
#  include <vector>
#endif

/* test for the pattern 4,0 in the base‑5 representation of x   */
static bool has_40(seq_t x)
{
    if (x < 5) return false;          /* need at least two digits */
    seq_t prev = x % 5;                /* least significant digit */
    x      /= 5;
    while (x > 0) {
        seq_t cur = x % 5;
        if (prev == 0 && cur == 4) return true;
        prev = cur;
        x   /= 5;
    }
    return false;
}

#if defined(__cplusplus)
/* C++ implementation – uses std::vector for dynamic storage   */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n<1 */

    static std::vector<seq_t> cache;
    static seq_t last_checked = 0;

    while ((idx_t)cache.size() < n) {
        seq_t cand = last_checked + 1;
        for (;;) {
            if (has_40(cand) && !has_40(cand + 1)) {
                cache.push_back(cand);
                last_checked = cand;
                break;
            }
            ++cand;
        }
    }
    return cache[n - 1];
}
#else
/* C implementation – uses manual malloc/free and a plain array  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n<1 */

    static seq_t *cache     = NULL;
    static idx_t  cache_cap = 0;
    static idx_t  cache_len = 0;
    static seq_t last_checked = 0;

    while (cache_len < n) {
        /* grow the array if needed */
        if (cache_len == cache_cap) {
            idx_t new_cap = (cache_cap == 0) ? 4 : cache_cap * 2;
            seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
            if (!new_cache) abort();          /* out of memory */
            if (cache) {
                /* copy existing elements */
                for (idx_t i = 0; i < cache_len; ++i)
                    new_cache[i] = cache[i];
                free(cache);
            }
            cache     = new_cache;
            cache_cap = new_cap;
        }

        seq_t cand = last_checked + 1;
        for (;;) {
            if (has_40(cand) && !has_40(cand + 1)) {
                cache[cache_len++] = cand;
                last_checked = cand;
                break;
            }
            ++cand;
        }
    }
    return cache[n - 1];
}
#endif
