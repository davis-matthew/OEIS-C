/*
 * A160357  Sign of first differences of Recamán's sequence A005132.
 * https://oeis.org/A160357
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = sign(A005132(n) - A005132(n-1))
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


static idx_t *rec_seq   = NULL;    /* Recamán sequence values */
static size_t rec_cap   = 0;
static idx_t rec_len    = 0;

static seq_t *a_vals   = NULL;    /* Sign of differences */
static size_t a_cap    = 0;
static idx_t a_len     = 0;

static bool *seen      = NULL;    /* bitmap of values already seen in Recamán */
static idx_t seen_cap  = 0;

/* Ensure rec_seq has space for at least 'need' elements */
static void grow_rec(idx_t need) {
    if (rec_cap < need) {
        size_t new_cap = rec_cap ? rec_cap * 2 : 8;
        while (new_cap < need) new_cap <<= 1;
        idx_t *tmp = (idx_t *)realloc(rec_seq, new_cap * sizeof(idx_t));
        rec_seq = tmp;
        rec_cap = new_cap;
    }
}

/* Ensure a_vals has space for at least 'need' elements */
static void grow_a(idx_t need) {
    if (a_cap < need) {
        size_t new_cap = a_cap ? a_cap * 2 : 8;
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(a_vals, new_cap * sizeof(seq_t));
        a_vals = tmp;
        a_cap  = new_cap;
    }
}

/* Ensure seen bitmap can index at least 'need' (i.e., indices 0 .. need-1) */
static void grow_seen(idx_t need) {
    if (seen_cap < need) {
        idx_t new_cap = seen_cap ? seen_cap * 2 : 1;
        while (new_cap < need) new_cap <<= 1;
        bool *tmp = (bool *)realloc(seen, new_cap * sizeof(bool));
        /* Zero‑initialize the new region */
        if (tmp != seen)
            memset(tmp + seen_cap, 0, (new_cap - seen_cap) * sizeof(bool));
        else
            memset(tmp + seen_cap, 0, (new_cap - seen_cap) * sizeof(bool));
        seen    = tmp;
        seen_cap = new_cap;
    }
}

seq_t a(idx_t n) {
    /* First time initialisation */
    if (rec_seq == NULL) {
        rec_cap = 8;
        rec_seq = (idx_t *)calloc(rec_cap, sizeof(idx_t));
        rec_seq[0] = (idx_t)0;
        rec_len    = 1;

        a_cap = 8;
        a_vals = (seq_t *)malloc(a_cap * sizeof(seq_t));
        a_vals[0] = (seq_t)1;     /* a(0) = 1 */
        a_len  = 1;

        seen_cap = 1;
        seen = (bool *)calloc(seen_cap, sizeof(bool));
        seen[0] = true;          /* 0 has appeared */
    }

    /* Extend precomputation up to required n */
    while (rec_len <= n) {
        idx_t i      = rec_len;               /* next index to compute (1‑based) */
        idx_t prev   = rec_seq[rec_len - 1];
        idx_t candidate = (prev >= i) ? prev - i : (idx_t)(-1);
        bool use_candidate = false;

        if (candidate != (idx_t)(-1)) {
            grow_seen(candidate + 1);
            if (!seen[candidate]) use_candidate = true;
        }

        idx_t next = use_candidate ? candidate : prev + i;

        /* Record the new Recamán value */
        grow_rec(rec_len + 1);
        rec_seq[rec_len] = next;

        /* Mark value as seen */
        grow_seen(next + 1);
        seen[next] = true;

        /* Sign of the difference */
        seq_t sign = (next > prev) ? (seq_t)1 : (seq_t)-1;

        /* Store the sign */
        grow_a(rec_len + 1);
        a_vals[rec_len] = sign;

        rec_len++;
        a_len++;
    }

    return a_vals[n];
}
