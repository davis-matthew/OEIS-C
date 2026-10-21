/*
 * A222562  Numbers that are highest in their respective Collatz (3x+1)
 *          trajectories only.
 * https://oeis.org/A222562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer m (offset 1) whose Collatz trajectory
 *            maximum equals m and for which no smaller starting integer has
 *            that same maximum. The function computes this by brute‑force
 *            search, caching results across calls
 * Range:     u8 n <= 13, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* static state for memoisation */
static seq_t *seq_vals   = NULL; /* sequence values a(1), a(2), … */
static size_t  seq_len   = 0;
static size_t  seq_cap   = 0;

static seq_t *max_seen   = NULL; /* maxima that have already appeared */
static size_t  seen_len  = 0;
static size_t  seen_cap  = 0;

static idx_t   cand      = 1;    /* next integer to evaluate */

/* Ensure capacity of a dynamic array, doubling as necessary */
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed > *cap) {
        size_t newcap = (*cap == 0) ? 8 : (*cap << 1);
        while (newcap < needed) newcap <<= 1;
        void *newptr = realloc(*ptr, newcap * elem_size);
        if (!newptr) exit(1);   /* fatal – allocation failed */
        *ptr = newptr;
        *cap = newcap;
    }
}

/* Test whether a value is already in the set of seen maxima */
static bool seen_contains(seq_t val)
{
    for (size_t i = 0; i < seen_len; ++i)
        if (max_seen[i] == val)
            return true;
    return false;
}

/* Add a value to the set of seen maxima if not present */
static void add_max_seen(seq_t val)
{
    if (!seen_contains(val)) {
        ensure_capacity(seen_len + 1, &seen_cap, (void **)&max_seen, sizeof(seq_t));
        max_seen[seen_len++] = val;
    }
}

seq_t a(idx_t n)
{
    /* behaviour for n < 1 is irrelevant – return 0 for safety */
    if (n == 0) return (seq_t)0;

    while (seq_len < (size_t)n) {
        seq_t m   = (seq_t)cand;        /* candidate starting number */
        seq_t cur = m;
        seq_t cur_max = cur;

        /* Collatz trajectory – stop when we reach 1 */
        while (cur != (seq_t)1) {
            if (cur & (seq_t)1)          /* odd: 3x + 1 */
                cur = cur * (seq_t)3 + (seq_t)1;
            else                         /* even: x / 2 */
                cur >>= 1;

            if (cur > cur_max) cur_max = cur;
        }

        /* Record m if it is highest in its own trajectory and this
           maximum has not appeared for a smaller starting integer. */
        if (cur_max == m && !seen_contains(m)) {
            ensure_capacity(seq_len + 1, &seq_cap, (void **)&seq_vals, sizeof(seq_t));
            seq_vals[seq_len++] = m;
        }

        /* Register the maximum reached for future uniqueness checks */
        add_max_seen(cur_max);

        ++cand;   /* next integer to examine */
    }

    return seq_vals[(size_t)n - 1];
}
