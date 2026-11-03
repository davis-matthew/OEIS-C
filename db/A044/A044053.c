/*
 * A044053  Numbers k such that string 0,1 occurs in the base 3 representation
 *          of k but not of k-1.
 * https://oeis.org/A044053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 5518, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9590 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A04453:
 * Numbers k such that the base‑3 representation of k contains the
 * substring "01" but the base‑3 representation of k−1 does not.
 * The implementation performs a brute‑force search, caching found
 * terms so that subsequent calls in any order are answered in
 * constant time.  No hard‑coded values are used. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Return true iff v's base‑3 representation contains "01". */
static bool has_01(seq_t v)
{
    if (v == 0) return false;
    seq_t prev = v % 3;
    v /= 3;
    while (v != 0) {
        seq_t cur = v % 3;
        if (prev == 1 && cur == 0) return true;
        prev = cur;
        v /= 3;
    }
    return false;
}

/* Cache of discovered sequence terms. */
static seq_t *cache      = NULL;   /* array of terms          */
static size_t cache_cap  = 0;      /* allocated capacity      */
static size_t cache_len  = 0;      /* number of terms stored  */
static seq_t next_k      = 1;      /* next integer to test    */

/* Ensure the cache contains at least n terms. */
static void ensure_cache(idx_t n)
{
    while (cache_len < n) {
        seq_t k = next_k++;          /* test next integer       */
        if (has_01(k) && !has_01(k - 1)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = k;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset is 1, return 0 for n<1 */
    ensure_cache(n);
    return cache[n - 1];
}
