/*
 * A022428  a(n) = c(n-1) + c(n-3) where c is the sequence of numbers not in
 *          a.
 * https://oeis.org/A022428
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_alloc, seq_cmp, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = c(n-1) + c(n-3), where c is the complement of a */
/* a(0) = 1, a(1) = 2, a(2) = 4, ...                                       */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/seq_cmp.h"

/* ---------- internal data ---------- */
static seq_t *a_arr = NULL;      /* a[0 .. a_len-1] */
static size_t a_len = 0, a_cap = 0;

static seq_t *c_arr = NULL;      /* c[0 .. c_len-1] */
static size_t c_len = 0, c_cap = 0;

static idx_t cur_candidate = 1;  /* next integer to test for membership in a */

/* ---------- helper functions ---------- */

/* Ensure a_arr has capacity for at least 'needed' elements */
static void
ensure_a_capacity(size_t needed)
{
    if (needed <= a_cap) return;
    size_t new_cap = a_cap ? a_cap << 1 : 8;
    while (new_cap < needed) new_cap <<= 1;
    a_arr = seq_realloc(a_arr, a_cap, new_cap);
    a_cap = new_cap;
}

/* Ensure c_arr has capacity for at least 'needed' elements */
static void
ensure_c_capacity(size_t needed)
{
    if (needed <= c_cap) return;
    size_t new_cap = c_cap ? c_cap << 1 : 8;
    while (new_cap < needed) new_cap <<= 1;
    c_arr = seq_realloc(c_arr, c_cap, new_cap);
    c_cap = new_cap;
}

/* Binary search: true if 'value' is already in a_arr */
static bool
a_contains(seq_t value)
{
    size_t lo = 0, hi = a_len;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        int cmp = seq_cmp(&a_arr[mid], &value);
        if (cmp == 0) return true;
        if (cmp < 0)
            lo = mid + 1;
        else
            hi = mid;
    }
    return false;
}

/* Generate the next element of c that is not in a_arr */
static void
generate_next_c(void)
{
    while (1) {
        seq_t cand = (seq_t)cur_candidate;
        if (!a_contains(cand)) {
            ensure_c_capacity(c_len + 1);
            c_arr[c_len++] = cand;
            cur_candidate++;           /* move to next integer for future searches */
            break;
        }
        cur_candidate++;               /* skip to next integer if already present */
    }
}

/* ---------- user function ---------- */

seq_t
a(idx_t n)
{
    /* Lazily initialise the sequences */
    if (!a_arr) {
        ensure_a_capacity(8);
        a_arr[0] = (seq_t)1;          /* a(0) */
        a_arr[1] = (seq_t)2;          /* a(1) */
        a_arr[2] = (seq_t)4;          /* a(2) */
        a_len = 3;
        /* c_arr starts empty; cur_candidate starts at 1 */
    }

    /* Build up to index n */
    while (a_len <= n) {
        idx_t k = a_len;                       /* index of new a value to compute */

        /* Ensure the complement array is large enough */
        while (c_len <= k - 1) {
            generate_next_c();
        }

        /* Compute a(k) = c(k-1) + c(k-3) */
        seq_t new_val = c_arr[k - 1] + c_arr[k - 3];

        /* Append the new value */
        ensure_a_capacity(a_len + 1);
        a_arr[a_len++] = new_val;
    }

    return a_arr[n];
}
