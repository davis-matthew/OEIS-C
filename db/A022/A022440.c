/*
 * A022440  a(n) = c(n-1) + c(n-3) where c is the sequence of positive numbers
 *          not in a.
 * https://oeis.org/A022440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This provides the OEIS A022440:
 * a(n) = c(n-1) + c(n-3),
 * where a(1)=3, a(2)=4, a(3)=5 and c is the sequence of positive integers
 * not appearing in a.  The sequence is generated incrementally; the
 * missing numbers c are kept in order, and ‘used’ records which numbers
 * have already appeared in a.  The algorithm works in O(n) time with
 * memory proportional to the largest a(n) seen. */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/*  Static state keeping the partially computed sequence and cache   */
/* ------------------------------------------------------------------ */
static seq_t *a_arr = NULL;          /* a[1..]  (a[0] unused)             */
static size_t a_cap = 0;             /* capacity of a_arr                */
static idx_t a_count = 0;            /* last computed index (>=0)        */

static seq_t *c_arr = NULL;          /* c[1..]   (c[0] unused)             */
static size_t c_cap = 0;             /* capacity of c_arr                */
static idx_t c_count = 0;            /* number of elements in c_arr      */

static unsigned char *used = NULL;   /* bitmap of numbers already in a   */
static size_t used_cap = 0;          /* capacity of used                 */

static seq_t next_missing = (seq_t)1;/* next number to test as missing    */

/* ------------------------------------------------------------------ */
/*  Helper to ensure array capacities                               */
/* ------------------------------------------------------------------ */
static inline void ensure_a_size(idx_t n) {
    /* n is inclusive: we need at least indices 0..n */
    if (a_cap <= (size_t)n) {
        size_t new_cap = a_cap ? a_cap * 2 : 4;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        a_arr = (seq_t *)realloc(a_arr, (new_cap + 1) * sizeof(seq_t));
        if (!a_arr) exit(1);
        a_cap = new_cap;
    }
}

static inline void ensure_c_size(idx_t n) {
    /* similar to above for c_arr */
    if (c_cap <= (size_t)n) {
        size_t new_cap = c_cap ? c_cap * 2 : 4;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        c_arr = (seq_t *)realloc(c_arr, (new_cap + 1) * sizeof(seq_t));
        if (!c_arr) exit(1);
        c_cap = new_cap;
    }
}

static inline void ensure_used_index(size_t idx) {
    if (used_cap <= idx) {
        size_t new_cap = used_cap ? used_cap * 2 : 16;
        if (new_cap <= idx) new_cap = idx + 1;
        used = (unsigned char *)realloc(used, (new_cap + 1) * sizeof(unsigned char));
        if (!used) exit(1);
        /* initialise new part to 0 */
        for (size_t i = used_cap + 1; i <= new_cap; ++i) used[i] = 0;
        used_cap = new_cap;
    }
}

/* ------------------------------------------------------------------ */
/*  Main sequence function                                           */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n) {
    if (n < 1) return 0;  /* undefined for n == 0, but harmless */

    /* compute values up to n */
    for (idx_t i = a_count + 1; i <= n; ++i) {
        if (i == 1) {
            ensure_a_size(1);
            a_arr[1] = (seq_t)3;
            a_count = 1;
            /* mark used */
            ensure_used_index(3);
            used[3] = 1;
            continue;
        }
        if (i == 2) {
            ensure_a_size(2);
            a_arr[2] = (seq_t)4;
            a_count = 2;
            ensure_used_index(4);
            used[4] = 1;
            continue;
        }
        if (i == 3) {
            ensure_a_size(3);
            a_arr[3] = (seq_t)5;
            a_count = 3;
            ensure_used_index(5);
            used[5] = 1;
            continue;
        }

        /* Ensure we have enough missing numbers c(i-1) and c(i-3) */
        while (c_count < i - 1) {
            /* find next missing candidate */
            size_t idx = (size_t)next_missing;
            ensure_used_index(idx);
            if (!used[idx]) {
                ensure_c_size(c_count + 1);
                c_arr[c_count + 1] = next_missing;
                c_count++;
            }
            next_missing++;
        }

        /* compute a(i) = c(i-1) + c(i-3) */
        seq_t val = c_arr[i - 1] + c_arr[i - 3];
        ensure_a_size(i);
        a_arr[i] = val;
        a_count = i;

        /* mark the value as used */
        size_t val_idx = (size_t)val;
        ensure_used_index(val_idx);
        used[val_idx] = 1;
    }

    return a_arr[(size_t)n];
}
