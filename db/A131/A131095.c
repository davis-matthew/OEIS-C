/*
 * A131095  Triangle where n-th row contains the smallest n positive integers
 *          (listed in order) with exactly n nonleading 0's in their binary
 *          representations and where the smallest term in the n-th row is >
 *          that the largest term in the (n-1)th row.
 * https://oeis.org/A131095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   sequence A131095 – obtain the n‑th term of the triangle where
 *            row r contains the smallest r integers whose binary
 *            representations have exactly r zeros (excluding the leading 1).
 *            The terms are listed row by row, starting with a(1)=2
 * Range:     u8 n <= 21, u16 n <= 105, u32 n <= 465, u64 n <= 1953,
 *            u128 n <= 7875, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/pow2.h"

/* Triangular sequence storage */
static seq_t *cache = NULL;          /* cache of computed terms, 1‑based index */
static size_t cache_len = 0;         /* number of terms in cache */
static idx_t cur_row = 0;            /* last fully processed row */
static seq_t prev_max = 0;           /* largest term already produced */

/* Grow the cache to at least `new_len` elements */
static void ensure_capacity(size_t new_len)
{
    if (new_len <= cache_len) return;
    size_t new_cap = cache_len ? cache_len << 1 : 64;
    while (new_cap < new_len) new_cap <<= 1;
    cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    /* no return value needed – just extend the cache */
}

/* Append a new term to the cache */
static void append_term(seq_t val)
{
    ensure_capacity(cache_len + 1);
    cache[cache_len++] = val;
}

/* Build rows until the cache contains at least `need` terms */
static void build_to_index(idx_t need)
{
    while (cache_len < (size_t)need) {
        ++cur_row;                  /* next row number */
        idx_t r = cur_row;
        idx_t collected = 0;        /* how many terms we have for this row */

        /* 1. Position with only one zero (len = r) */
        seq_t val0 = pow2((idx_t)r);           /* 2^r */
        if (val0 > prev_max) {
            append_term(val0);
            prev_max = val0;
            ++collected;
        }

        /* 2. Positions with exactly one '1' among (r+1) bits */
        if (collected < r) {
            seq_t base = pow2((idx_t)(r + 1)); /* 2^(r+1) */
            for (idx_t k = 0; k <= r && collected < r; ++k) {
                seq_t val = base + pow2(k);   /* place single '1' at position k */
                if (val > prev_max) {
                    append_term(val);
                    prev_max = val;
                    ++collected;
                }
            }
        }

        /* The above is always sufficient: 
           len = r yields 1 term (if > prev_max),
           len = r+1 yields r+1 terms, all > prev_max, so r terms can be selected.
        */
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;       /* undefined for indices < 1, return 0 */
    if (cache_len < (size_t)n) build_to_index(n);
    return cache[n - 1];               /* cache is 0‑based, sequence is 1‑based */
}
