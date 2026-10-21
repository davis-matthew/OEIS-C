/*
 * A058336  a(n) = least value such that sequence increases and pairwise
 *          differences are unique.
 * https://oeis.org/A058336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least integer > a(n-1) such that all pairwise differences
 *            of the sequence up to n are distinct, with a(1)=1, a(2)=5
 * Range:     u8 n <= 16, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

static seq_t  *seq_array = NULL;   /* stored sequence values          */
static size_t  seq_cap   = 0;      /* allocated capacity (seq_t items)*/
static idx_t   seq_len   = 0;      /* number of terms computed        */

static uint64_t *diff_bits = NULL; /* bitset of used differences      */
static size_t   diff_cap   = 0;    /* number of 64‑bit words allocated */

/* Ensure seq_array has capacity for at least 'needed' entries. */
static void ensure_seq_cap(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(seq_array, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    seq_array = tmp;
    seq_cap   = new_cap;
}

/* Ensure diff_bits has room for word index 'word_index'. */
static void ensure_diff_cap(size_t word_index)
{
    if (word_index < diff_cap) return;
    size_t new_cap = diff_cap ? diff_cap * 2 : 64;
    while (new_cap <= word_index) new_cap *= 2;
    uint64_t *tmp = (uint64_t *)realloc(diff_bits, new_cap * sizeof(uint64_t));
    if (!tmp) abort();
    if (new_cap > diff_cap)
        memset(tmp + diff_cap, 0, (new_cap - diff_cap) * sizeof(uint64_t));
    diff_bits = tmp;
    diff_cap  = new_cap;
}

/* Mark difference 'd' as used. */
static inline void mark_diff_used(seq_t d)
{
    size_t word = (size_t)(d >> 6);
    size_t bit  = (size_t)(d & 63);
    ensure_diff_cap(word);
    diff_bits[word] |= ((uint64_t)1 << bit);
}

/* Test whether difference 'd' has already been used. */
static inline bool is_diff_used(seq_t d)
{
    size_t word = (size_t)(d >> 6);
    if (word >= diff_cap) return false;
    size_t bit = (size_t)(d & 63);
    return (diff_bits[word] >> bit) & 1U;
}

/* Public function required by the assignment */
seq_t a(idx_t n)
{
    /* Lazy initialization of the sequence cache */
    if (seq_len == 0) {
        /* Ensure space for the first two terms */
        ensure_seq_cap(2);
        seq_array[0] = (seq_t)1;
        seq_array[1] = (seq_t)5;
        seq_len = 2;

        /* Allocate and clear the difference bitset */
        diff_cap = 64;
        diff_bits = (uint64_t *)calloc(diff_cap, sizeof(uint64_t));
        if (!diff_bits) abort();

        /* Difference between the first two terms */
        mark_diff_used((seq_t)4);
    }

    /* Extend the sequence up to the requested index */
    while (seq_len < n) {
        seq_t prev = seq_array[seq_len - 1];
        seq_t cand = prev + 1;          /* candidate must be > prev */

        for (;;) {
            bool ok = true;
            for (idx_t i = 0; i < seq_len; ++i) {
                seq_t d = cand - seq_array[i];
                if (is_diff_used(d)) { ok = false; break; }
            }
            if (ok) break;              /* found the next term */
            ++cand;
        }

        /* Record all new differences appearing with this candidate */
        for (idx_t i = 0; i < seq_len; ++i)
            mark_diff_used(cand - seq_array[i]);

        /* Store the new term, expanding storage if necessary */
        ensure_seq_cap(seq_len + 1);
        seq_array[seq_len++] = cand;
    }

    return seq_array[(size_t)(n - 1)];
}
