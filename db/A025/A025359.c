/*
 * A025359  Numbers that are the sum of 4 nonzero squares in exactly 3 ways.
 * https://oeis.org/A025359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 65, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for numbers that are the sum of 4 non‑zero squares in exactly
   3 unordered ways.                                     */
/*    a(n) = the nth integer > 0 with exactly 3 representations
   as a^2 + b^2 + c^2 + d^2, with a,b,c,d >= 1 and a<=b<=c<=d   */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* integer square root (floor) for uint64_t values; uses binary search
   and __uint128_t for safe multiplication. */
static idx_t _isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq == n) return mid;
        if (sq > n) {
            if (mid == 0) break;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return high;
}

/* Compute all numbers <= limit that are sums of 4 non‑zero squares
   in exactly three unordered ways.  Return a sorted array of those
   numbers and store the count in *out_cnt.  Caller must free the
   returned array. */
static idx_t *compute_terms(idx_t limit, size_t *out_cnt)
{
    /* Allocate count array; one byte per possible sum. */
    uint8_t *cnt = (uint8_t *)calloc(limit + 1, sizeof(uint8_t));
    if (!cnt) return NULL;

    idx_t sq_lim = _isqrt_u64(limit);

    for (idx_t a = 1; a <= sq_lim; ++a) {
        idx_t a2 = a * a;
        if (a2 > limit) break;

        for (idx_t b = a; b <= sq_lim; ++b) {
            idx_t b2 = b * b;
            idx_t s2 = a2 + b2;
            if (s2 > limit) break;

            for (idx_t c = b; c <= sq_lim; ++c) {
                idx_t c2 = c * c;
                idx_t s3 = s2 + c2;
                if (s3 > limit) break;

                idx_t rem = limit - s3;
                idx_t dmax = _isqrt_u64(rem);
                if (dmax < c) continue;

                for (idx_t d = c; d <= dmax; ++d) {
                    idx_t sum = s3 + d * d;
                    /* sum <= limit by construction */
                    if (cnt[sum] < 255) cnt[sum]++;
                }
            }
        }
    }

    /* Count how many numbers have exactly three representations. */
    size_t num_terms = 0;
    for (idx_t v = 1; v <= limit; ++v)
        if (cnt[v] == 3) ++num_terms;

    /* Collect those numbers in order. */
    idx_t *terms = (idx_t *)malloc(num_terms * sizeof(idx_t));
    if (!terms) {
        free(cnt);
        *out_cnt = 0;
        return NULL;
    }

    size_t pos = 0;
    for (idx_t v = 1; v <= limit; ++v)
        if (cnt[v] == 3) terms[pos++] = v;

    free(cnt);
    *out_cnt = num_terms;
    return terms;
}

/* Public sequence function.  Maintains a cache that is resized
   when more terms are required. */
seq_t a(idx_t n)
{
    /* Static cache state. */
    static idx_t *terms      = NULL;
    static size_t term_count = 0;
    static idx_t computed_limit = 0;

    /* Extend search until we have at least n terms. */
    while (term_count < n) {
        /* Choose next limit for search. */
        idx_t next_limit = computed_limit ? computed_limit * 2 : 200;
        if (next_limit <= computed_limit) next_limit = computed_limit + 1;

        size_t new_cnt = 0;
        idx_t *new_terms = compute_terms(next_limit, &new_cnt);
        if (!new_terms) {
            /* On allocation error, return 0; caller cannot detect. */
            return (seq_t)0;
        }

        /* Replace old cache. */
        if (terms) free(terms);
        terms = new_terms;
        term_count = new_cnt;
        computed_limit = next_limit;
    }

    return (seq_t)terms[n - 1];
}
