/*
 * A062932  a(0) = 0; a(n) = smallest number > a(n-1) such that a(n-1)+a(n) is
 *          a palindrome.
 * https://oeis.org/A062932
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 733, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/**
 * Formula: a(0) = 0; for n > 0,
 * a(n) = smallest integer > a(n‑1) such that a(n‑1)+a(n) is a
 * decimal palindrome.
 *
 * The function keeps a static cache of already computed values
 * so that arbitrary call order is supported.  The cache is
 * extended only when necessary.  Palindrome checking is done
 * via arithmetic (no string conversions), which works for
 * every build type of seq_t (built‑in integer or arbitrary
 * precision).
 */

#include <stdlib.h>
#include <stdbool.h>

/* ---------- helper: decimal palindrome test ---------- */
static bool is_palindrome(seq_t x)
{
    seq_t rev = 0;
    seq_t orig = x;
    while (orig > 0) {
        rev = rev * 10 + (orig % 10);
        orig = orig / 10;
    }
    return rev == x;
}

/* ---------- public sequence function ---------- */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* cached terms  a(0)…a(k) */
    static size_t cache_len = 0;         /* allocated length */
    static idx_t computed_up_to = 0;     /* last index with a valid entry */

    /* initialise cache with a(0)=0 on first call */
    if (cache_len == 0) {
        cache_len = 1;
        cache = (seq_t*)malloc(sizeof(seq_t));
        cache[0] = 0;                    /* a(0) = 0 */
        computed_up_to = 0;
    }

    /* enlarge cache if needed */
    if (n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *new_cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) {
            /* allocation failure – in the test harness this should never happen,
               but we guard just in case */
            return 0;
        }
        /* zero initialise the newly allocated part (not strictly needed) */
        for (size_t i = cache_len; i < new_len; ++i)
            new_cache[i] = 0;
        cache = new_cache;
        cache_len = new_len;
    }

    /* compute any missing terms up to n */
    while (computed_up_to < n) {
        seq_t prev = cache[computed_up_to];
        seq_t cand = prev + 1;                        /* candidate > previous */
        while (!is_palindrome(prev + cand))
            ++cand;
        cache[computed_up_to + 1] = cand;
        ++computed_up_to;
    }

    return cache[(size_t)n];
}
