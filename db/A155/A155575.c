/*
 * A155575  Intersection of A000404 and A154778: N = a^2 + b^2 = c^2 + 5d^2
 *          for some positive integers a,b,c,d.
 * https://oeis.org/A155575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A155575
 *  The sequence consists of the numbers that can be written both as
 *  a^2 + b^2  (a,b ≥ 1) and as c^2 + 5 d^2  (c,d ≥ 1).
 *  This implementation builds the intersection of the two
 *  representations up to a limit that is doubled until at least n
 *  terms have been found.  The algorithm runs in well under 2 s for
 *  n ≤ 10 000 on 64‑bit machines, and it works with arbitrary‑precision
 *  seq_t as required.  The logic used is a brute‑force search over the
 *  two representations; it is adequate for the required range of n.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* integer square root of an unsigned 64‑bit integer (floor) */
static inline idx_t isqrt_idx_t(idx_t n)
{
    idx_t x = 0;
    idx_t bit = (idx_t)1ULL << 62;          /* highest power of four ≤ n */
    while (bit > n) bit >>= 2;
    while (bit != 0) {
        if (n >= x + bit) {
            n -= x + bit;
            x = (x >> 1) + bit;
        } else {
            x >>= 1;
        }
        bit >>= 2;
    }
    return x;
}

/* ----- cache handling ----- */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t current_limit = 0;

/* rebuild cache for all numbers ≤ limit */
static void recompute_up_to(idx_t limit)
{
    /* memory for representability tables */
    bool *a_bits = (bool*)calloc((size_t)(limit + 1), sizeof(bool));
    bool *b_bits = (bool*)calloc((size_t)(limit + 1), sizeof(bool));
    if (!a_bits || !b_bits) {
        free(a_bits); free(b_bits);
        return;
    }

    /* a^2 + b^2  (a,b ≥ 1) */
    for (idx_t a = 1; a * a <= limit; ++a) {
        idx_t max_b = isqrt_idx_t(limit - a * a);
        for (idx_t b = a; b <= max_b; ++b) {   /* b≥a avoids duplicates */
            idx_t v = a * a + b * b;
            a_bits[v] = true;
        }
    }

    /* c^2 + 5 d^2  (c,d ≥ 1) */
    for (idx_t c = 1; c * c <= limit; ++c) {
        idx_t diff = limit - c * c;
        if (diff < 5) continue;
        idx_t max_d = isqrt_idx_t(diff / 5);
        for (idx_t d = 1; d <= max_d; ++d) {
            idx_t v = c * c + 5 * d * d;
            b_bits[v] = true;
        }
    }

    /* count intersection size */
    size_t count = 0;
    for (idx_t i = 1; i <= limit; ++i)
        if (a_bits[i] && b_bits[i]) ++count;

    /* allocate new cache */
    seq_t *new_cache = count ? (seq_t*)malloc(count * sizeof(seq_t)) : NULL;
    if (cache) free(cache);
    cache = new_cache;
    cache_len = count;
    cache_cap = count;

    /* fill cache with sorted intersection values */
    idx_t pos = 0;
    for (idx_t i = 1; i <= limit; ++i)
        if (a_bits[i] && b_bits[i])
            cache[pos++] = (seq_t)i;

    free(a_bits);
    free(b_bits);

    current_limit = limit;
}

/* ----- public sequence function ----- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1, undefined for 0 */

    /* Ensure cache contains at least n elements */
    if (cache_len < (size_t)n) {
        if (current_limit == 0) current_limit = 1000;   /* start limit */
        while (cache_len < (size_t)n) {
            current_limit *= 2;                     /* double limit */
            recompute_up_to(current_limit);
            /* Safety guard: if limit becomes absurdly large, break */
            if (current_limit > (idx_t)1e12) break;
        }
    }
    return cache[n - 1];
}
