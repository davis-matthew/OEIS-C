/*
 * A184478  Lower s-Wythoff sequence, where s(n) = 3n + 1. Complement of
 *          A184479.
 * https://oeis.org/A184478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS A184478 – Lower s-Wythoff sequence for s(n)=3n+1.
 * a(n) is the smallest positive integer not yet used, where the set of
 * used numbers consists of the a(k) terms and the paired numbers a(k)+s(k)
 * for all k < n. The implementation uses a lazy incremental sieve.
 * Complexity: O(n + max(a(n))) per call, which is well below 2s for n≈10000.
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *a_seq = NULL;     /* array of computed a(k) values   */
static size_t a_len = 0;        /* number of values stored         */
static size_t a_cap = 0;        /* allocated capacity of a_seq     */

static uint8_t *used = NULL;    /* bitmap of used numbers (0/1)    */
static size_t used_cap = 0;     /* allocated capacity of used      */

/* Grow the used bitmap to accommodate indices < new_cap. Zero the newly
 * added portion of the array. */
static void ensure_used_capacity(size_t new_cap) {
    if (new_cap <= used_cap) return;
    size_t cap = used_cap ? used_cap : 64;
    while (cap < new_cap) cap <<= 1;
    uint8_t *new_used = (uint8_t *)realloc(used, cap);
    if (!new_used) abort();            /* out of memory */
    if (new_cap > used_cap) {
        /* Zero the new part of the array */
        memset(new_used + used_cap, 0, new_cap - used_cap);
    }
    used = new_used;
    used_cap = cap;
}

/* Grow the a_seq array to accommodate indices < new_cap. */
static void ensure_a_capacity(size_t new_cap) {
    if (new_cap <= a_cap) return;
    size_t cap = a_cap ? a_cap : 16;
    while (cap < new_cap) cap <<= 1;
    seq_t *new_a = (seq_t *)realloc(a_seq, cap * sizeof(seq_t));
    if (!new_a) abort();
    a_seq = new_a;
    a_cap = cap;
}

/* Lazy computation of the sequence up to index n. */
seq_t a(idx_t n) {
    if (n == 0) {          /* invalid index; defined as 0 */
        return (seq_t)0;
    }
    /* Ensure we have computed all values up to n. */
    while (a_len < (size_t)n) {
        /* Natural index for the next term (1‑based). */
        idx_t k = (idx_t)a_len + 1;           /* current k */
        seq_t s = (seq_t)(3 * k + 1);         /* s(k)=3k+1      */
        /* Find smallest unused positive integer. */
        static idx_t next_candidate = 1;      /* remembers first free number */
        idx_t cand = next_candidate;
        for (;;) {
            /* Expand bitmap if needed. */
            ensure_used_capacity((size_t)cand + 1);
            if (!used[(size_t)cand]) break;
            ++cand;
        }
        seq_t a_val = (seq_t)cand;            /* the term a(k)          */
        /* Store the value. */
        ensure_a_capacity(a_len + 1);
        a_seq[a_len] = a_val;
        ++a_len;
        // Mark the two numbers as used.
        ensure_used_capacity((size_t)cand + 1);
        used[(size_t)cand] = 1;
        /* a_val + s might be larger than the current bitmap. */
        idx_t pair_idx = (idx_t)(a_val + s);
        ensure_used_capacity((size_t)pair_idx + 1);
        used[(size_t)pair_idx] = 1;
        /* Next search starts just after the candidate we just used. */
        next_candidate = cand + 1;
    }
    return a_seq[(size_t)n - 1];
}
