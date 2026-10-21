/*
 * A224761  a(2)=10; thereafter a(n) = smallest number m such that a(n-1)+m =
 *          (a(n-1) followed by the leading digit of m).
 * https://oeis.org/A224761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 10; for n>1, a(n) is the smallest m such that * a(n-1)+m
 *            = (a(n-1) followed by the leading digit of m). This * equivalent
 *            recurrence can be simplified to: * a(n) = 9*a(n-1)+d * where d
 *            is the leading digit of a(n). For each d=1..9 we test * whether
 *            the leading digit of m = 9*a(n-1)+d is in...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Helper: leading digit of a seq_t number (assumes seq_t supports division). */
static int leading_digit(seq_t x)
{
    while (x >= (seq_t)10) {
        x /= (seq_t)10;
    }
    return (int)x; /* fits in an int because x < 10 */
}

/* Static cache of computed terms. */
static seq_t *cache = NULL;      /* array of terms, 0-based: a(1) is cache[0] */
static idx_t cached_last = 0;    /* number of terms currently stored */

/* Grow the cache capacity if needed. */
static void ensure_cache_capacity(idx_t needed)
{
    static idx_t capacity = 0;
    if (needed <= capacity)
        return;
    /* Double capacity until large enough. */
    size_t new_cap = capacity ? capacity * 2 : 16;
    while (new_cap < (size_t)needed)
        new_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) {
        /* Allocation failed; terminate program. */
        abort();
    }
    cache = new_cache;
    capacity = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* Undefined for n=0; return 0 as safe fallback. */
        return (seq_t)0;
    }

    /* Ensure cache is large enough. */
    if (cached_last < n) {
        ensure_cache_capacity(n);
        /* Initialize first term if not already done. */
        if (cached_last == 0) {
            cache[0] = (seq_t)10;      /* a(1) */
            cached_last = 1;
        }
        /* Compute remaining terms up to n. */
        for (idx_t idx = cached_last; idx < n; ++idx) {
            seq_t prev = cache[idx - 1];          /* a(idx) is previous term */
            seq_t base = (seq_t)9 * prev;          /* 9*a(idx) */
            seq_t m_candidate = 0;
            /* Try d from 1 to 9; first valid gives minimal m. */
            for (int d = 1; d <= 9; ++d) {
                m_candidate = base + (seq_t)d;    /* m = 9*prev + d */
                if (leading_digit(m_candidate) == d) {
                    break;                      /* found minimal m */
                }
            }
            cache[idx] = m_candidate;            /* store a(idx+1) */
        }
        cached_last = n;
    }

    return cache[n - 1];    /* a(1) is at index 0 */
}
