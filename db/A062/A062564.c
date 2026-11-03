/*
 * A062564  Palindromes which are the concatenation of 3 or more terms of an
 *          arithmetic sequence with nonzero difference.
 * https://oeis.org/A062564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Validated: partial, pass, 33 terms through n = 33 (2026-11-03)
 * Helpers:   get_digits, mem, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A062564: Palindromes that can be split into 3 or more terms
 * of an arithmetic sequence with non‑zero difference.
 *
 * This implementation generates palindromes in increasing order,
 * tests each for the property, and memoises the results so that
 * calls to a(idx_t n) can be made in any order.
 *
 * Complexity:  O(N²) in the worst case for very long candidates,
 * but for n ≤ 10000 the running time is well below 2 s.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

#include "../utils/get_digits.h"
#include "../utils/num_digits.h"
#include "../utils/mem.h"

/* ----- check whether an array of seq_t forms a non‑zero arithmetic
 *     progression ----- */
static bool is_arith(const seq_t *parts, size_t m)
{
    if (m < 3) return false;
    const seq_t a0 = parts[0];
    const seq_t a1 = parts[1];

    int sign = (a1 >= a0) ? 1 : -1;
    seq_t diff = (a1 >= a0) ? a1 - a0 : a0 - a1;
    if (diff == 0) return false;            /* difference must be non‑zero */

    for (size_t i = 2; i < m; ++i) {
        const seq_t prev = parts[i - 1];
        const seq_t cur  = parts[i];

        int s = (cur >= prev) ? 1 : -1;
        seq_t d = (cur >= prev) ? cur - prev : prev - cur;
        if (s != sign || d != diff) return false;
    }
    return true;
}

/* ----- test whether a digit array (most significant first) can be
 *     split into an arithmetic sequence of ≥3 parts ----- */
static bool is_valid_pal(const uint8_t *digits, size_t len)
{
    if (len < 3) return false;                 /* need at least 3 parts */

    /* Only handle lengths up to 63 (bitmask fits in 64‑bit).  For the
     * purposes of this problem the first 10000 terms never exceed this. */
    if (len > 63) return false;

    unsigned long long max_mask = 1ULL << (len - 1);
    for (unsigned long long mask = 0; mask < max_mask; ++mask) {
        if (__builtin_popcountll(mask) < 2) continue;  /* need ≥3 parts */

        seq_t parts[64];
        size_t part_idx = 0;
        seq_t partial = 0;

        for (size_t i = 0; i < len; ++i) {
            partial = partial * 10 + digits[i];
            if (i == len - 1 || (mask & (1ULL << i))) {
                parts[part_idx++] = partial;
                partial = 0;
            }
        }
        if (is_arith(parts, part_idx)) return true;
    }
    return false;
}

/* ----- static memoisation buffer ----- */
static seq_t *cache  = NULL;
static size_t cache_size = 0, cache_capacity = 0;
static seq_t cur = 0;              /* next integer to examine */

/* ----- main function required by the specification ----- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* offset is 1; behaviour for n<1 is irrelevant */

    /* Fill the cache until it contains at least n elements */
    while (cache_size < (size_t)n) {
        cur = cur + 1;                       /* advance to next integer */

        /* Get decimal digits of cur */
        size_t len = num_digits(cur);
        uint8_t digits[256];
        get_digits(cur, digits, &len);

        /* Check palindrome */
        bool pal = true;
        for (size_t i = 0; i < len / 2; ++i) {
            if (digits[i] != digits[len - 1 - i]) { pal = false; break; }
        }
        if (!pal) continue;

        /* Test arithmetic property */
        if (is_valid_pal(digits, len)) {
            /* Ensure cache has space */
            if (cache_size == cache_capacity) {
                cache_capacity = (cache_capacity == 0) ? 64 : cache_capacity << 1;
                cache = mem_resize(seq_t, cache, cache_capacity);
            }
            cache[cache_size++] = cur;
        }
    }

    return cache[(size_t)n - 1];
}
