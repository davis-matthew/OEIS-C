/*
 * A044666  Numbers n such that string 3,7 occurs in the base 9 representation
 *          of n but not of n+1.
 * https://oeis.org/A044666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044666: numbers n whose base‑9 representation
 * contains the substring "37" but whose base‑9 representation of n+1
 * does not.  Returns the n‑th such number (a(1) is 34). */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Helper: test whether a number contains the substring 3,7 in base 9. */
static bool has_37_base9(seq_t x)
{
    int prev = -1;               /* previous (lower) digit */
    while (x) {
        int d = (int)(x % 9);
        if (d == 3 && prev == 7) /* high digit 3 followed by low digit 7 */
            return true;
        prev = d;
        x /= 9;
    }
    return false;
}

/* Dynamic cache of the sequence. */
static seq_t *cache     = NULL;  /* array of found terms */
static size_t cache_len = 0;     /* how many terms stored */
static size_t cache_cap = 0;     /* allocated capacity */
static seq_t   cur       = 0;    /* current number to test */

/* Ensure capacity for at least min_cap elements. */
static void cache_ensure_capacity(size_t min_cap)
{
    if (min_cap <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    if (new_cap < min_cap)
        new_cap = min_cap;
    seq_t *newp = mem_new(seq_t, new_cap);
    for (size_t i = 0; i < cache_len; ++i)
        newp[i] = cache[i];
    mem_free(cache);
    cache      = newp;
    cache_cap  = new_cap;
}

/* Compute terms until cache contains at least n elements. */
static void generate_until(size_t n)
{
    if (cache_len >= n)
        return;
    cache_ensure_capacity(cache_len + n);  /* optimistic reserve */

    while (cache_len < n) {
        ++cur;  /* next candidate */
        if (has_37_base9(cur) && !has_37_base9(cur + 1))
            cache[cache_len++] = cur;
    }
}

/* Main public function requested. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* undefined for offset 1, return 0 for safety */
    generate_until((size_t)n);
    return cache[n-1];
}
