/*
 * A083171  a(n) = the arithmetic mean of the n-th row of triangle A083167.
 * https://oeis.org/A083171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A083170(n)/n, where A083170(n) is the sum of row n of
 *            triangle A083167 (computed by direct construction of the
 *            triangle)
 * Range:     u8 n <= 6, u16 n <= 36, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 49 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static inline idx_t A083167_bit_index(seq_t v)  { return (idx_t)(v & (seq_t)63); }
static inline idx_t A083167_block_index(seq_t v) { return (idx_t)(v >> 6); }

static seq_t *A083167_term_cache = NULL;
static idx_t  A083167_cache_cap   = 0;
static idx_t  A083167_terms_gen   = 0;
static idx_t  A083167_rows_gen    = 0;

static void A083167_ensure_cache_capacity(idx_t need)
{
    if (A083167_terms_gen + need <= A083167_cache_cap) return;
    size_t new_cap = A083167_cache_cap ? (size_t)A083167_cache_cap * 2 : 128;
    while ((size_t)(A083167_terms_gen + need) > new_cap) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, A083167_term_cache, new_cap);
    if (!tmp) exit(1);
    A083167_term_cache = tmp;
    A083167_cache_cap  = (idx_t)new_cap;
}

static uint64_t *A083167_used_bits = NULL;
static idx_t    A083167_used_bits_cap = 0;

static void A083167_ensure_used_capacity(seq_t v)
{
    idx_t block = A083167_block_index(v);
    while (block >= A083167_used_bits_cap) {
        size_t new_cap = A083167_used_bits_cap ? (size_t)A083167_used_bits_cap * 2 : 128;
        if ((size_t)block >= new_cap) new_cap = (size_t)block + 1;
        uint64_t *tmp = (uint64_t *)realloc(A083167_used_bits, new_cap * sizeof(uint64_t));
        if (!tmp) exit(1);
        for (size_t i = (size_t)A083167_used_bits_cap; i < new_cap; ++i) tmp[i] = 0ULL;
        A083167_used_bits     = tmp;
        A083167_used_bits_cap = (idx_t)new_cap;
    }
}

static inline bool A083167_is_used(seq_t v)
{
    idx_t block = A083167_block_index(v);
    if (block >= A083167_used_bits_cap) return false;
    return (A083167_used_bits[block] >> A083167_bit_index(v)) & 1ULL;
}

static void A083167_set_used(seq_t v)
{
    A083167_ensure_used_capacity(v);
    idx_t block = A083167_block_index(v);
    A083167_used_bits[block] |= 1ULL << A083167_bit_index(v);
}

static seq_t A083167_a(idx_t n)
{
    if (n == 0) return 0;

    while (A083167_terms_gen < n) {
        idx_t r = A083167_rows_gen + 1;

        if (r == 1) {
            A083167_ensure_cache_capacity(1);
            A083167_term_cache[A083167_terms_gen++] = (seq_t)1;
            A083167_set_used((seq_t)1);
        } else {
            seq_t base = (seq_t)(r - 1);

            seq_t *row_vals = mem_new(seq_t, (size_t)r);
            if (!row_vals) exit(1);

            idx_t taken = 0;
            seq_t sum  = (seq_t)0;

            seq_t k = (seq_t)1;
            while (taken < r - 1) {
                seq_t val = base * k;
                if (!A083167_is_used(val)) {
                    row_vals[taken++] = val;
                    sum += val;
                }
                ++k;
            }

            seq_t mod    = sum % (seq_t)r;
            seq_t target = ((seq_t)r - mod) % (seq_t)r;
            seq_t last_candidate = 0;
            k = (seq_t)1;
            while (true) {
                seq_t val = base * k;
                if (A083167_is_used(val))          { ++k; continue; }

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

            A083167_ensure_cache_capacity(r);
            for (idx_t i = 0; i < r; ++i) {
                A083167_term_cache[A083167_terms_gen] = row_vals[i];
                A083167_set_used(row_vals[i]);
                ++A083167_terms_gen;
            }
            mem_free(row_vals);
        }
        ++A083167_rows_gen;
    }
    return A083167_term_cache[n - 1];
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    idx_t start = n * (n - 1) / 2;   /* terms before row n */
    seq_t sum = (seq_t)0;
    for (idx_t i = 1; i <= n; ++i) {
        sum += A083167_a(start + i);
    }
    return sum / (seq_t)n;
}
