/*
 * A037318  Numbers whose base-4 and base-7 expansions have the same digit
 *          sum.
 * https://oeis.org/A037318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses a search: iterate over integers and test digit sums in base-4 and base-7. */
#include <stdlib.h>
#include <stdbool.h>

/* Helper: compute digit sum of n in given base (small base). */
static int digit_sum_base(seq_t n, unsigned base)
{
    int sum = 0;
    while (n > 0) {
        sum += (int)(n % base);
        n /= base;
    }
    return sum;
}

/* Cached sequence storage */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Ensure cache capacity for at least 'need' elements */
static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap == 0 ? 128 : cache_cap;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();  /* out of memory */
    cache = new_cache;
    cache_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* Under offset, behavior undefined; return 0 to avoid crash. */
        return (seq_t)0;
    }

    /* Grow sequence until we have at least n terms. */
    ensure_cache_cap(n);
    seq_t cur = cache_len == 0 ? (seq_t)1 : cache[cache_len - 1] + (seq_t)1;

    while (cache_len < (size_t)n) {
        if (digit_sum_base(cur, 4) == digit_sum_base(cur, 7)) {
            cache[cache_len++] = cur;
        }
        cur++;
    }

    return cache[n - 1];
}
