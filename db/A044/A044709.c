/*
 * A044709  Numbers n such that string 8,5 occurs in the base 9 representation
 *          of n but not of n+1.
 * https://oeis.org/A044709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th smallest integer whose base‑9 representation
 *            contains the digit sequence 8,5 and the next integer does not
 * Range:     u8 n <= 3, u16 n <= 889, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 61 terms through n = 991 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Return true if 'n' in base 9 has the consecutive digits 8,5
   (i.e. 8 followed immediately by 5 in the usual high‑to‑low order). */
static bool has85(seq_t n)
{
    if (n == 0)
        return false;
    seq_t prev = n % 9;   /* least significant digit */
    n /= 9;
    while (n > 0) {
        seq_t cur = n % 9;   /* next higher digit */
        if (cur == 8 && prev == 5)
            return true;
        prev = cur;
        n /= 9;
    }
    return false;
}

/* Cache for previously computed terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Next candidate to test, initially the first term of the sequence */
static seq_t cur_val = 77;

/* Ensure the cache can hold at least 'needed' elements */
static void ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    cache = tmp;
    cache_cap = new_cap;
}

/* Sequence generator */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for zero index; safe guard */

    /* Generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        /* Scan integers until we find the next valid term */
        while (true) {
            if (has85(cur_val) && !has85(cur_val + 1)) {
                ensure_cache_cap(cache_len + 1);
                cache[cache_len++] = cur_val;
                cur_val++;   /* Next search starts after the found term */
                break;
            }
            cur_val++;
        }
    }
    return cache[n - 1];
}
