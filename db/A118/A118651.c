/*
 * A118651  Numbers k such that k^2 is a palindrome when written in base 17.
 * https://oeis.org/A118651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = k where k^2 is a palindrome when written in base 17 (OEIS
 *            A118651). The function returns the n-th term (a(1) = 0)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 51, u64 n <= 100, u128 n <= 100,
 *            big any n
 * Validated: standard, pass, 61 terms through n = 100 (2026-11-03)
 * Helpers:   is_palindrome_base, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation performs an incremental search and caches results for
   repeated calls.  It uses the supplied is_palindrome utility and a
   generic grow_seq helper to manage the cache. */
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_palindrome_base.h"
#include "../utils/mem.h"

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    static seq_t *cache     = NULL;   /* cache of found k values   */
    static size_t cache_cap = 0;      /* allocated capacity       */
    static size_t cache_len = 0;      /* number of elements in cache */
    static idx_t  next_k    = 0;      /* next k to test            */

    /* Extend cache until we have at least n elements */
    while (cache_len < (size_t)n) {
        seq_t kseq = (seq_t)next_k;
        seq_t sq   = kseq * kseq;           /* k²  */
        if (is_palindrome_base(sq, (idx_t)17)) {
            grow_seq(&cache, &cache_cap, cache_len + 1);
            cache[cache_len++] = kseq;
        }
        ++next_k;
    }

    return cache[(size_t)n - 1];
}
