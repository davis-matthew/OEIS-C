/*
 * A044684  Numbers n such that string 5,7 occurs in the base 9 representation
 *          of n but not of n+1.
 * https://oeis.org/A044684
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

/* Sequence A044684:  Numbers n such that the string "57" occurs in the base‑9
 * representation of n but not in that of n+1.
 * The function works by iteratively searching for each subsequent term
 * and memoising the results.  The search scans the base‑9 digits from least
 * significant to most significant, so the pattern 5 followed by 7 (MSD→LSD)
 * is detected by checking whether a lower digit is 7 and the next higher
 * digit is 5. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Test whether the base‑9 representation of x contains the consecutive
 * digits 5 followed by 7 (reading most significant to least significant). */
static inline bool has57(seq_t x)
{
    if (x == 0)
        return false;
    seq_t prev = 0;     /* previously seen digit (lower place) */
    bool has_prev = false;
    while (x > 0) {
        seq_t digit = x % 9;   /* current (higher) digit */
        if (has_prev && prev == 7 && digit == 5)
            return true;
        prev = digit;
        has_prev = true;
        x /= 9;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* sequence offset is 1 */
        return 0;

    /* Cache of already found terms */
    static seq_t *cache = NULL;
    static idx_t cache_len  = 0;
    static idx_t cache_cap  = 0;
    static seq_t last_candidate = 0;   /* last number examined */

    /* Return cached value if already computed */
    if (n <= cache_len)
        return cache[n - 1];

    /* Grow the cache to hold at least n terms */
    if (cache_cap == 0)
        cache_cap = 1;
    while (cache_cap < n)
        cache_cap <<= 1;
    if (cache == NULL || cache_cap > cache_len) {
        cache = mem_resize(seq_t, cache, cache_cap);
    }

    /* Find successive terms until the n‑th one is reached */
    while (cache_len < n) {
        seq_t candidate = last_candidate + 1;
        while (true) {
            if (has57(candidate) && !has57(candidate + 1))
                break;
            candidate += 1;
        }
        cache[cache_len] = candidate;
        last_candidate  = candidate;
        cache_len += 1;
    }

    return cache[n - 1];
}
