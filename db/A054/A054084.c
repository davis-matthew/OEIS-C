/*
 * A054084  Permutation of N: for each k >= 1, let p(k)=least natural number
 *          not already an a(i), q(k)=p(k)+k, a(2k-1)=q(k), a(2k)=p(k).
 * https://oeis.org/A054084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, eigen
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A054084: permutation of natural numbers.
   For each k ≥ 1 let p(k) = the least natural number not already used,
   let q(k) = p(k) + k,
   then a(2k‑1) = q(k), a(2k) = p(k). */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* ---------- static state ---------- */
static seq_t   *seq = NULL;        /* sequence values */
static size_t   seq_len = 0;       /* number of values currently stored */
static size_t   seq_cap = 0;       /* capacity of seq array */

static bool    *used = NULL;      /* flags for used natural numbers */
static size_t   used_cap = 0;      /* capacity of used array */
static size_t   next_candidate = 1;/* next natural number to try for p(k) */

/* enlarge the used array so that index i (inclusive) exists and is false */
static void ensure_used_index(size_t i)
{
    if (i < used_cap) return;
    size_t new_cap = used_cap ? used_cap : 16;
    while (new_cap <= i) new_cap <<= 1;
    bool *t = (bool *)realloc(used, new_cap * sizeof(bool));
    if (!t) abort();
    used = t;
    /* initialize newly allocated portion to false */
    for (size_t j = used_cap; j < new_cap; ++j) used[j] = false;
    used_cap = new_cap;
}

/* enlarge the sequence array so that it can hold at least need elements */
static void seq_grow(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap : 16;
    while (new_cap < need) new_cap <<= 1;
    seq_t *t = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!t) abort();
    seq = t;
    seq_cap = new_cap;
}

/* compute a(n) for any n ≥ 1, with memoisation */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;         /* undefined, guard */
    size_t target = (size_t)n;           /* maximum index needed */

    while (seq_len < target)
    {
        /* Determine the next k (1‑based), based on how many pairs we already have. */
        size_t k = seq_len / 2 + 1;       /* integer division floors */

        /* Find next unused natural number for p(k) */
        while (next_candidate < used_cap && used[next_candidate]) ++next_candidate;
        if (next_candidate >= used_cap) ensure_used_index(next_candidate);
        size_t p_idx = next_candidate;            /* natural number value */
        seq_t p_val = (seq_t)p_idx;                /* convert to seq_t */

        /* Compute q(k) = p(k) + k */
        seq_t q_val = p_val + (seq_t)k;
        size_t q_idx = (size_t)q_val;             /* expect fits in size_t */
        if (q_idx >= used_cap) ensure_used_index(q_idx);

        /* Mark both numbers as used */
        used[p_idx] = true;
        used[q_idx] = true;

        /* Grow the sequence buffer for two new values */
        seq_grow(seq_len + 2);

        /* Append in required order: a(2k‑1)=q(k), a(2k)=p(k) */
        seq[seq_len++] = q_val;
        seq[seq_len++] = p_val;

        /* Advance candidate for the next p(k) */
        ++next_candidate;
    }

    return seq[target - 1];            /* zero‑based indexing into seq */
}
