/*
 * A168139  Squares whose sum and number of digits are also squares.
 * https://oeis.org/A168139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 21, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   digit_len, digit_sum, grow_seq, isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A168139: a(n) = the n-th integer that is a perfect square,
   whose decimal digit sum and number of decimal digits are both perfect
   squares. */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"
#include "../utils/digit_len.h"
#include "../utils/isqrt_seq.h"
#include "../utils/grow_seq.h"

/* Helper: true if v is a perfect square. */
static bool is_square_seq(seq_t v)
{
    seq_t r = isqrt_seq(v);
    return r * r == v;
}

/* Static cache of discovered terms. */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static size_t cache_len  = 0;
static idx_t   last_m    = 0;   /* next m to examine (m^2) */

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* Undefined; return 0 for safety. */

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        /* Compute (last_m)^2 safely. */
        uint64_t m      = (uint64_t)last_m;
        uint64_t m2     = m * m;                 /* product fits in 64 bits */
        seq_t   sq      = (seq_t)m2;             /* cast to seq_t */

        /* Detect overflow into seq_t (occurs for small seq_t types). */
        if ((uint64_t)sq != m2) {
            /* Further values will also overflow; stop generating. */
            break;
        }

        /* Check that digit sum is a square. */
        seq_t dsum = digit_sum(sq);
        if (!is_square_seq(dsum)) {
            ++last_m;
            continue;
        }

        /* Check that number of digits is a square. */
        int  diglen = digit_len(sq);
        if (!is_square_seq((seq_t)diglen)) {
            ++last_m;
            continue;
        }

        /* All conditions satisfied -- append to cache. */
        if (cache_len == cache_cap) {
            grow_seq(&cache, &cache_cap, cache_len + 1);
        }
        cache[cache_len++] = sq;
        ++last_m;
    }

    /* If we reached the desired index, return the cached value. */
    if (cache_len >= n) {
        return cache[n - 1];
    }

    /* If we ran out of representable values before reaching n, return 0. */
    return 0;
}
