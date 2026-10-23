/*
 * A043745  Numbers whose number of runs in the base-2 representation is
 *          congruent to 6 mod 7.
 * https://oeis.org/A043745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th integer whose number of runs in the binary representation is
 * congruent to 6 modulo 7.  The function keeps an internal cache of already
 * found terms so that calls with arbitrary n are answered in O(1) after the
 * required terms have been generated. */
#include <stdlib.h>

static inline idx_t count_runs(idx_t x)
{
    /* Count maximal runs of equal bits in the binary representation of x. */
    if (x == 0) return 1;   /* unused, but safe */
    idx_t run = 1;
    /* Find position of the most significant '1' bit. */
    idx_t msb = 0;
    idx_t tmp = x;
    while (tmp) {
        ++msb;
        tmp >>= 1;
    }
    idx_t prev = (x >> (msb - 1)) & 1;
    for (long long i = (long long)msb - 2; i >= 0; --i) {
        idx_t curr = (x >> i) & 1;
        if (curr != prev) {
            ++run;
            prev = curr;
        }
    }
    return run;
}

static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t cache_capacity = 0;
static idx_t last_checked = 0;   /* last integer that was examined */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1, but this value is never used */
    /* Grow cache until it contains at least n terms */
    while (cache_size < n) {
        ++last_checked;
        if (count_runs(last_checked) % 7 == 6) {
            if (cache_size == cache_capacity) {
                size_t new_cap = cache_capacity == 0 ? 64 : cache_capacity * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {
                    /* In a real build, handle out‑of‑memory appropriately. */
                    abort();
                }
                cache = tmp;
                cache_capacity = new_cap;
            }
            cache[cache_size++] = (seq_t) last_checked;
        }
    }
    return cache[n - 1];
}
