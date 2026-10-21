/*
 * A083167  Triangle read by rows in which from the second row onwards the
 *          n-th row contains n smallest multiples of (n-1) not included
 *          earlier such that the row sum is a multiple of n. Triangle begins
 *          with 1.
 * https://oeis.org/A083167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   bit_index, block_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Procedure:  computes A083167.
   Triangle read by rows: row 1 is {1}.
   For r >= 2 the r‑th row consists of the r smallest multiples of (r-1)
   that are not in earlier rows such that the sum of the row is divisible
   by r. The sequence is the flattening of this triangle. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/bit_index.h"
#include "../utils/block_index.h"

/* cache of produced terms (flattened triangle) */
static seq_t *term_cache = NULL;   /* array of terms  */
static idx_t  cache_cap   = 0;      /* capacity of term_cache  */
static idx_t  terms_gen   = 0;      /* number of terms cached   */
static idx_t  rows_gen    = 0;      /* rows already produced   */

/* ensure the cache can hold at least 'need' additional terms */
static void ensure_cache_capacity(idx_t need)
{
    if (terms_gen + need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 128;
    while (terms_gen + need > new_cap) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(term_cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    term_cache = tmp;
    cache_cap  = (idx_t)new_cap;
}

/* dynamic bitset of used values */
static uint64_t *used_bits = NULL;     /* each bit represents an integer value  */
static idx_t    used_bits_cap = 0;     /* number of 64‑bit blocks */

/* ensure the bitset has room for value 'v' */
static void ensure_used_capacity(seq_t v)
{
    idx_t block = block_index(v);
    while (block >= used_bits_cap) {
        size_t new_cap = used_bits_cap ? used_bits_cap * 2 : 128;
        if ((size_t)block >= new_cap) new_cap = block + 1;
        uint64_t *tmp = (uint64_t *)realloc(used_bits, new_cap * sizeof(uint64_t));
        if (!tmp) exit(1);
        /* clear new blocks */
        for (size_t i = used_bits_cap; i < new_cap; ++i) tmp[i] = 0ULL;
        used_bits     = tmp;
        used_bits_cap = (idx_t)new_cap;
    }
}

/* test whether a value has already been used */
static inline bool is_used(seq_t v)
{
    idx_t block = block_index(v);
    if (block >= used_bits_cap) return false;
    return (used_bits[block] >> bit_index(v)) & 1ULL;
}

/* mark a value as used */
static void set_used(seq_t v)
{
    ensure_used_capacity(v);
    idx_t block = block_index(v);
    used_bits[block] |= 1ULL << bit_index(v);
}

/* main function that returns the n‑th term  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* below offset – harmless */

    /* generate rows until we have at least n terms cached */
    while (terms_gen < n) {
        idx_t r = rows_gen + 1;      /* row number to generate */

        if (r == 1) {                /* first row is just 1 */
            ensure_cache_capacity(1);
            term_cache[terms_gen++] = (seq_t)1;
            set_used((seq_t)1);
        } else {
            seq_t base = (seq_t)(r - 1);   /* common factor = r-1 */

            /* array to hold this row's values */
            seq_t *row_vals = (seq_t *)malloc((size_t)r * sizeof(seq_t));
            if (!row_vals) exit(1);

            idx_t taken = 0;
            seq_t sum  = (seq_t)0;

            /* collect the first r-1 smallest unused multiples of base */
            seq_t k = (seq_t)1;
            while (taken < r - 1) {
                seq_t val = base * k;
                if (!is_used(val)) {
                    row_vals[taken++] = val;
                    sum += val;
                }
                ++k;
            }

            /* find the last element so that the row sum is divisible by r */
            seq_t mod    = sum % (seq_t)r;
            seq_t target = ((seq_t)r - mod) % (seq_t)r;
            seq_t last_candidate = 0;
            k = (seq_t)1;
            while (true) {
                seq_t val = base * k;
                if (is_used(val))          { ++k; continue; }

                /* avoid duplicating a value chosen earlier in this row */
                bool dup = false;
                for (idx_t i = 0; i < taken; ++i) {
                    if (row_vals[i] == val) { dup = true; break; }
                }
                if (dup) { ++k; continue; }

                if ((val % (seq_t)r) == target) {
                    last_candidate = val;
                    break;
                }
                ++k;
            }
            row_vals[taken] = last_candidate;

            /* append the row to the cache and mark values as used */
            ensure_cache_capacity(r);
            for (idx_t i = 0; i < r; ++i) {
                term_cache[terms_gen] = row_vals[i];
                set_used(row_vals[i]);
                ++terms_gen;
            }
            free(row_vals);
        }
        ++rows_gen;          /* next row number */
    }
    return term_cache[n - 1];
}
