/*
 * A119474  a(2^m + k) = a(k)-th integer among those positive integers not
 *          occurring earlier in the sequence, where m >= 0, 1 <= k<= 2^m.
 *          a(1) = 1.
 * https://oeis.org/A119474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 181, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A119474
   a(1) = 1
   For n≥2, let m = ⌊log₂(n‑1)⌋, k = n – 2ᵐ  (1 ≤ k ≤ 2ᵐ).
   a(n) is the a(k)-th positive integer that has NOT appeared earlier
   in the sequence.  The sequence is a permutation of ℕ.  */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/floor_log2_uint64.h"

/* Static cache for computed terms: 1‑indexed logically. */
static seq_t     *seq_cache = NULL;
static size_t    seq_cap   = 0;
static size_t    seq_len   = 0;

/* Static cache of used values: 1‑indexed bitset. */
static uint8_t   *used_cache = NULL;   /* 1 = used, 0 = unused */
static size_t    used_cap   = 0;       /* current allocated size */

/* Ensure used_cache can index up to 'need'.  */
static void ensure_used_cap(size_t need)
{
    if (used_cap >= need) return;
    size_t new_cap = used_cap ? used_cap << 1 : 4;
    while (new_cap < need) new_cap <<= 1;
    uint8_t *new_ptr = (uint8_t*)realloc(used_cache, (new_cap + 1) * sizeof(uint8_t));
    if (!new_ptr) return;               /* out of memory – sequence will be incomplete */
    if (used_cache)                     /* zero newly added portion */
        memset(new_ptr + used_cap + 1,
               0, (new_cap - used_cap) * sizeof(uint8_t));
    used_cache = new_ptr;
    used_cap   = new_cap;
}

/* Ensure seq_cache has capacity at least 'need'.  */
static void ensure_seq_cap(size_t need)
{
    if (seq_cap >= need) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 4;
    while (new_cap < need) new_cap <<= 1;
    seq_t *new_ptr = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!new_ptr) return;               /* out of memory */
    seq_cache = new_ptr;
    seq_cap   = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for offset 0 */

    /* Generate terms up to n if needed. */
    if (seq_len < (size_t)n) {
        ensure_seq_cap(n);
        if (!used_cache) {           /* first time initialisation */
            used_cache = (uint8_t*)calloc(5, sizeof(uint8_t)); /* 1‑based, indices 0‑4 used */
            used_cap   = 4;
        }

        if (seq_len == 0) {          /* a(1) = 1 */
            seq_cache[0] = 1;
            seq_len = 1;
            ensure_used_cap(1);
            used_cache[1] = 1;
        }

        /* Compute remaining terms. */
        for (size_t i = seq_len + 1; i <= (size_t)n; ++i) {
            idx_t m = floor_log2_uint64(i - 1);           /* floor(log₂(n-1)) */
            idx_t pow2 = (idx_t)((unsigned long long)1 << m);
            idx_t k = i - pow2;                            /* 1 ≤ k ≤ 2ᵐ */
            seq_t idx = seq_cache[k - 1];                  /* a(k) */

            /* Find the idx‑th missing positive integer. */
            seq_t missing = 0;
            seq_t missing_cnt = 0;
            seq_t candidate = 1;

            while (missing_cnt < idx) {
                size_t cand_idx = (size_t)candidate;     /* assume fits in size_t */
                if (cand_idx > used_cap) ensure_used_cap(cand_idx);
                if (!used_cache[cand_idx]) {
                    ++missing_cnt;
                    if (missing_cnt == idx) {
                        missing = candidate;
                        break;
                    }
                }
                ++candidate;
            }

            seq_cache[i - 1] = missing;                  /* store term */
            size_t miss_idx = (size_t)missing;
            if (miss_idx > used_cap) ensure_used_cap(miss_idx);
            used_cache[miss_idx] = 1;                    /* mark as used */
        }
        seq_len = n;
    }

    return seq_cache[n - 1];
}
