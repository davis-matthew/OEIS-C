/*
 * A043689  a(n) = (s(n)-1)/2, where s(n) is the n-th number whose base-2
 *          representation has exactly 7 runs.
 * https://oeis.org/A043689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043689:
   a(n) = (s(n)-1)/2
   where s(n) is the nth integer whose binary representation
   contains exactly 7 runs.
 */
#include <stddef.h>
#include <stdlib.h>

static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;
static seq_t last_checked = 0;   /* last integer examined */

/* Ensure storage for at least needed elements */
static void cache_grow(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the number of runs in the binary expansion of x */
static int run_count(seq_t x)
{
    if (x == 0)
        return 0;               /* undefined for 0 but harmless */

    int runs = 1;                /* first bit starts a run */
    seq_t prev = x & 1;          /* least‑significant bit */
    x >>= 1;                     /* remove it */

    while (x != 0) {
        seq_t cur = x & 1;
        if (cur != prev) {
            ++runs;
            prev = cur;
        }
        x >>= 1;
    }
    return runs;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* outside the sequence offset */

    /* Generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        ++last_checked;
        if (run_count(last_checked) == 7) {
            cache_grow(cache_len + 1);
            cache[cache_len++] = (last_checked - 1) / 2;
        }
    }
    return cache[(size_t)n - 1];
}
