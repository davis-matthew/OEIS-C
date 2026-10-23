/*
 * A119797  Numbers m such that m and m+1 have the same number of distinct
 *          digits in decimal representation.
 * https://oeis.org/A119797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers m such that m and m+1 have the same number of distinct
 *            decimal digits. The function incrementally searches m = 0, 1, 2
 *            … and memoises all terms found so far. It works for any seq_t
 *            and idx_t that support the required arithmetic operations
 * Range:     u8 n <= 184, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Count distinct decimal digits of x. */
static unsigned int distinct_digits(seq_t x)
{
    if (x == (seq_t)0) return 1;          /* "0" → 1 distinct digit */
    unsigned char seen[10] = {0};
    unsigned int cnt = 0;
    while (x != (seq_t)0) {
        unsigned int d = (unsigned int)(x % (seq_t)10); /* 0–9 */
        if (!seen[d]) {
            seen[d] = 1;
            ++cnt;
        }
        x /= (seq_t)10;
    }
    return cnt;
}

/* Static cache of previously found sequence terms. */
static seq_t  *cache     = NULL;
static size_t cache_cap = 0;   /* allocated number of elements */
static size_t cache_len = 0;   /* number of terms currently stored  */
static seq_t  next_checked = 0; /* next integer to test for the property */

/* Return the n‑th term (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;  /* undefined for n=0 but harmless */
    }

    /* 0‑based target index. */
    size_t target = (size_t)(n - (idx_t)1);

    /* Grow the cache until it contains the target term. */
    while (cache_len <= target) {
        seq_t m  = next_checked;
        seq_t mp = m + (seq_t)1;

        if (distinct_digits(m) == distinct_digits(mp)) {
            /* Ensure room in the cache. */
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap > 0) ? cache_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(cache,
                                               new_cap * sizeof(seq_t));
                if (!tmp) {
                    /* Allocation failed – abort with zero. */
                    return (seq_t)0;
                }
                cache     = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = m;
        }
        ++next_checked;
    }

    return cache[target];
}
