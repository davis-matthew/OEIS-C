/*
 * A038676  Fourth powers ending in a (different) positive fourth power.
 * https://oeis.org/A038676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 165, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A038676: Fourth powers ending in a different positive fourth power.
 * 
 * The function a(n) generates the first n terms of the sequence by
 * examining successive fourth powers m⁴ (m ≥ 1).  
 * A term is kept if and only if some proper non‑empty suffix of m⁴ equals
 * a smaller positive fourth power k⁴ (k < m).
 * The result is memoised in a static cache so that calls arrive in any
 * order.
 * All arithmetic uses only the supported operators of seq_t (integer
 * multiplication, division, modulo, comparison).  No external utilities
 * are required beyond the standard headers.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Function prototype expected by the build system */
seq_t a(idx_t n);

/* ------------- Helper functions ------------- */

/* Ensure the dynamic array has room for needed elements.
 * Pointer is updated if realloc occurs. */
static void
ensure_capacity(seq_t **arr, size_t *len, size_t *cap, size_t need)
{
    if (need > *cap) {
        size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

/* Return the number of decimal digits of x (x > 0). */
static int
digit_len(seq_t x)
{
    int len = 0;
    do {
        ++len;
        x /= (seq_t)10;
    } while (x != (seq_t)0);
    return len;
}

/* Return whether suffix of val equals any earlier fourth power. */
static bool
has_smaller_power_suffix(seq_t val, seq_t *pow4, size_t pow4_len)
{
    /* No suffix possible for a single‑digit number. */
    int d = digit_len(val);
    if (d <= 1) return false;

    seq_t pow10L = (seq_t)10;          /* 10¹ */
    for (int L = 1; L < d; ++L) {
        seq_t suffix = val % pow10L;   /* last L digits */

        /* Linear search – pow4 is sorted by construction. */
        for (size_t i = 0; i < pow4_len; ++i) {
            if (pow4[i] == suffix) return true;
        }

        pow10L *= (seq_t)10;          /* next power of ten */
    }
    return false;
}

/* ------------- Main sequence function ------------- */

seq_t a(idx_t n)
{
    /* n == 0 is outside the offset; return 0 for safety. */
    if (n == (idx_t)0) return (seq_t)0;

    /* Static state for memoisation. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;

    static seq_t *pow4 = NULL;       /* m⁴ values for k < current m */
    static size_t pow4_len = 0, pow4_cap = 0;

    static idx_t m_next = (idx_t)2;  /* next m to examine */

    /* One‑time initialisation of the static arrays. */
    if (!cache) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
    }
    if (!pow4) {
        pow4_cap = 16;
        pow4 = (seq_t *)malloc(pow4_cap * sizeof(seq_t));
        /* initialise k = 1 → 1⁴ = 1 */
        pow4[0] = (seq_t)1;
        pow4_len = 1;
    }

    /* Fill the cache until it contains at least n terms. */
    while (cache_len < (size_t)n) {
        /* Compute current m⁴. */
        seq_t m = (seq_t)m_next;
        seq_t val = m * m;   /* m² */
        val *= val;          /* (m²)² = m⁴ */

        /* Check if a valid suffix exists. */
        if (has_smaller_power_suffix(val, pow4, pow4_len)) {
            /* Append to cache. */
            ensure_capacity(&cache, &cache_len, &cache_cap, cache_len + 1);
            cache[cache_len++] = val;
        }

        /* Append current m⁴ to pow4 for future suffix checks. */
        ensure_capacity(&pow4, &pow4_len, &pow4_cap, pow4_len + 1);
        pow4[pow4_len++] = val;

        /* Advance to next m. */
        ++m_next;
    }

    return cache[n - 1];
}
