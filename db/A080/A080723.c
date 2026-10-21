/*
 * A080723  a(0) = 1; for n>0, a(n) is taken to be the smallest positive
 *          integer greater than a(n-1) which is consistent with the condition
 *          "n is a member of the sequence if and only if a(n) == 1 mod 3".
 * https://oeis.org/A080723
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(a(n)) = 3*n + 4, a(0) = 1
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   ensure_bool_cap
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates the OEIS sequence A080723.               */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/ensure_bool_cap.h"

/* -------------- Dynamic array helpers -------------- */
static void ensure_seq_cap(seq_t **arr, size_t *cap, size_t needed) {
    if (needed <= *cap) return;
    size_t old_cap = *cap;
    size_t new_cap = old_cap ? old_cap : 16;
    while (new_cap <= needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    *arr = tmp;
    if (new_cap > old_cap)
        memset((*arr) + old_cap, 0, (new_cap - old_cap) * sizeof(seq_t));
    *cap = new_cap;
}

/* -------------- Sequence storage -------------- */
static seq_t *a_seq      = NULL;   /* a[n]  */
static size_t  a_cap      = 0;
static size_t  a_len      = 0;      /* smallest n not yet computed */

static seq_t *a_target   = NULL;   /* a_target[n] = known value of a(n) if set                   */
static size_t  target_cap = 0;

static bool   *seen_vals  = NULL;   /* seen_vals[v] = true if value v has appeared in a_seq   */
static size_t  seen_cap   = 0;

/* -------------- Initialization -------------- */
static void init_sequence(void) {
    if (a_seq) return; /* already initialized */

    ensure_seq_cap(&a_seq,   &a_cap,   32);
    ensure_seq_cap(&a_target,&target_cap,32);
    ensure_bool_cap(&seen_vals,&seen_cap,32);

    /* a(0) = 1 */
    a_seq[0] = (seq_t)1;
    a_len = 1;

    /* mark 1 as seen */
    if ((size_t)1 >= seen_cap) ensure_bool_cap(&seen_vals, &seen_cap, (size_t)1 + 1);
    seen_vals[1] = true;

    /* set target for index 1: a(1) = 4 (since 1 = a(0)) */
    if ((size_t)1 >= target_cap) ensure_seq_cap(&a_target, &target_cap, (size_t)1 + 1);
    a_target[1] = (seq_t)4;
}

/* -------------- Main function -------------- */
seq_t a(idx_t n) {
    init_sequence();

    while (a_len <= (size_t)n) {
        idx_t idx = (idx_t)a_len;   /* index to compute next */

        seq_t val;
        if (idx < target_cap && a_target[idx] != (seq_t)0) {
            /* value forced by earlier a(a(k)) relation */
            val = a_target[idx];
        } else {
            /* choose the smallest number > previous that satisfies the property */
            seq_t cand = a_seq[idx - 1] + (seq_t)1;
            bool needOne = false;
            if ((size_t)idx < seen_cap) needOne = seen_vals[idx];
            while ( ((cand % 3) == 1) != needOne ) cand += 1;
            val = cand;
        }

        /* store computed value */
        if ((size_t)idx >= a_cap) ensure_seq_cap(&a_seq, &a_cap, (size_t)idx + 1);
        a_seq[(size_t)idx] = val;

        /* mark value as seen */
        if ((size_t)val >= seen_cap) ensure_bool_cap(&seen_vals, &seen_cap, (size_t)val + 1);
        seen_vals[(size_t)val] = true;

        /* set target for index val: a(val) = 3*idx + 4 */
        seq_t targ = (seq_t)idx * (seq_t)3 + (seq_t)4;
        if ((size_t)val >= target_cap) ensure_seq_cap(&a_target, &target_cap, (size_t)val + 1);
        a_target[(size_t)val] = targ;

        ++a_len;
    }

    return a_seq[(size_t)n];
}
