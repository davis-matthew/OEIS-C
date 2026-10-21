/*
 * A239221  Numbers n such that n^3 is divisible by each of its nonzero
 *          digits.
 * https://oeis.org/A239221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 280, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_frequencies
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A239221: numbers n such that n³ is divisible by each of its
 * non‑zero decimal digits.  The function iteratively tests successive
 * integers and memoises the valid ones. */
 
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_frequencies.h"

/* Cache of terms found so far.  Elements are stored as idx_t (uint64_t). */
static idx_t *cache = NULL;
static size_t cap = 0;      /* capacity of cache */
static size_t len = 0;      /* number of numbers stored */
static idx_t next = 1;      /* next candidate to test */

/* Grow the cache to hold at least ‘need’ elements. */
static void ensure_cap(size_t need)
{
    if (need <= cap) return;
    size_t new_cap = cap ? (cap << 1) : 16;
    while (new_cap < need) new_cap <<= 1;
    cache = (idx_t *)realloc(cache, new_cap * sizeof(idx_t));
    cap = new_cap;
}

/* Test whether n satisfies the property. */
static bool valid(idx_t n)
{
    seq_t cube = (seq_t)n * (seq_t)n * (seq_t)n;
    unsigned int freq[10] = {0};

    digit_frequencies(cube, freq);
    for (int d = 1; d <= 9; ++d) {
        if (freq[d] && (cube % d) != 0)
            return false;
    }
    return true;
}

/* Return the n‑th term of the sequence (1‑indexed). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety for n < 1 */

    while (len < n) {
        if (valid(next)) {
            ensure_cap(len + 1);
            cache[len++] = next;
        }
        ++next;
    }
    return (seq_t)cache[n - 1];
}
