/*
 * A184483  Upper s-Wythoff sequence, where s(n)=3n-1. Complement of A184482.
 * https://oeis.org/A184483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the Upper s‑Wythoff sequence with s(n) = 3n‑1.
   For each n it finds the smallest unused integer l(n), then
   a(n) = l(n) + 3n - 1.  All used integers (both lower and upper)
   are marked to avoid reuse.  Results are memoised. */

/* Required headers */
#include <stdlib.h>
#include <stdbool.h>

/* --- bitmap for used integers -------------------------------------------- */
static uint8_t *used_bitmap = NULL;   /* bitset of used integers */
static size_t  used_bits   = 0;       /* number of bits allocated */

/* tests whether integer k (as idx_t) is marked as used */
static inline bool is_used(idx_t k) {
    if ((size_t)k >= used_bits) return false;
    size_t byte = (size_t)k >> 3;
    return (used_bitmap[byte] >> (k & 7)) & 1u;
}

/* marks integer k (as idx_t) as used, reallocating if necessary */
static inline void set_used(idx_t k) {
    if (!used_bitmap) {
        used_bitmap = (uint8_t*)calloc(1, 1);
        used_bits   = 1;
    }
    if ((size_t)k >= used_bits) {
        size_t new_bits  = (size_t)k + 1;
        size_t new_bytes = (new_bits + 7) >> 3;
        size_t old_bytes = (used_bits + 7) >> 3;
        uint8_t *tmp = (uint8_t*)realloc(used_bitmap, new_bytes);
        if (!tmp) abort();
        if (new_bytes > old_bytes)
            memset(tmp + old_bytes, 0, new_bytes - old_bytes);
        used_bitmap = tmp;
        used_bits   = new_bits;
    }
    size_t byte = (size_t)k >> 3;
    used_bitmap[byte] |= (1u << (k & 7));
}

/* --- cache of already‑computed upper sequence values --------------------- */
static seq_t *upper_cache = NULL;
static size_t upper_cap   = 0;
static size_t upper_len   = 0;

/* next candidate for the lower part of the pair (smallest unused integer) */
static idx_t next_candidate = 1;

/* ensures upper_cache holds at least 'need' elements */
static inline void ensure_upper_cap(size_t need) {
    if (need <= upper_cap) return;
    size_t new_cap = upper_cap ? (upper_cap << 1) : 16;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(upper_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    upper_cache = tmp;
    upper_cap   = new_cap;
}

/* -------------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    /* generate terms until we have at least n */
    while (upper_len < (size_t)n) {
        /* find smallest unused integer for the lower part */
        while (is_used((idx_t)next_candidate)) ++next_candidate;

        seq_t lower   = (seq_t)next_candidate;
        idx_t idx     = (idx_t)(upper_len + 1);           /* 1‑based index */
        seq_t offset  = 3 * (seq_t)idx - 1;              /* s(idx) = 3*idx-1 */
        seq_t upper   = lower + offset;

        /* mark both numbers as used */
        set_used((idx_t)lower);
        set_used((idx_t)upper);

        ++next_candidate;

        /* store the upper value in the cache */
        ensure_upper_cap(upper_len + 1);
        upper_cache[upper_len++] = upper;
    }
    return upper_cache[(size_t)(n - 1)];
}
