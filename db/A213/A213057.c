/*
 * A213057  Triangle read by rows: left edge is all 1's, right edge is 1, 2,
 *          3, 4, ...; construct an internal entry by concatenating the two
 *          entries above it.
 * https://oeis.org/A213057
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Build the "word Pascal triangle" of OEIS A213057. Each entry is
 *            the decimal concatenation of the two numbers above it. The left
 *            edge of each row is all 1's, the right edge is 1, 2, 3, …. The
 *            sequence a(n) is the row‑major traversal of this triangle
 * Range:     u8 n <= 10, u16 n <= 11, u32 n <= 22, u64 n <= 29, u128 n <= 35,
 *            big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

#include "../utils/pow10.h"
#include "../utils/digit_len.h"

/* Static cache for previously computed terms */
static seq_t *cache   = NULL;   /* array of computed values */
static size_t cache_len = 0;    /* number of values in cache */
static size_t cache_cap = 0;    /* allocated capacity */

static seq_t *prev_row = NULL; /* previous row (length = next_row) */
static idx_t next_row  = 0;     /* next row index to compute */

/* Return a(n) for any n >= 0.  Cache is extended lazily. */
seq_t a(idx_t n)
{
    /* Grow the cache until it contains the requested index. */
    while (cache_len <= n) {
        size_t len = (size_t)next_row + 1;
        seq_t *curr = (seq_t *)malloc(len * sizeof(seq_t));
        if (!curr) abort();                       /* out of memory */

        /* Left edge is always 1. */
        curr[0] = (seq_t)1;

        /* Right edge is the row number + 1 (1‑based). */
        curr[len-1] = (seq_t)(next_row + 1);

        /* Internal entries are concatenations of two values above. */
        for (size_t j = 1; j + 1 < len; ++j) {
            seq_t left  = prev_row[j-1];
            seq_t right = prev_row[j];

            /* Number of decimal digits of the right operand. */
            idx_t dl = (idx_t)digit_len(right);

            /* left * 10^dl + right   (concatenation). */
            seq_t pow_mul = pow10(dl);
            curr[j] = left * pow_mul + right;
        }

        /* Append the new row to the global cache. */
        if (cache_len + len > cache_cap) {
            cache_cap = (cache_cap > 0 ? cache_cap * 2 : 64);
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            if (!cache) abort();                     /* out of memory */
        }
        memcpy(cache + cache_len, curr, len * sizeof(seq_t));
        cache_len += len;

        /* Move to the next row. */
        free(prev_row);
        prev_row = curr;
        ++next_row;
    }

    return cache[(size_t)n];
}
