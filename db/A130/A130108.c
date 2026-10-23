/*
 * A130108  Rearrangement of natural numbers such that each three terms sum up
 *          to a perfect square.
 * https://oeis.org/A130108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: Starting with an empty permutation, repeat the following
 * step: find the lexicographically smallest triple (a<b<c) of positive
 * integers not yet used whose sum a+b+c is a perfect square. Append a,
 * b, c to the permutation.  The nth term a(n) is the nth element of
 * that permutation (OEIS A130108).
 * 
 * Initial values (a(1)–a(24)):
 * 1, 2, 6, 3, 4, 9, 5, 7, 13, 8, 10, 18, 11, 12, 26, 14, 15, 20, 16,
 * 17, 31, 19, 21, 24
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

/* static storage for memoisation */
static seq_t *seq       = NULL;  /* sequence array */
static size_t  seq_cap  = 0;     /* allocated capacity of seq   */
static size_t  seq_len  = 0;     /* number of stored values     */

static bool *used       = NULL;  /* used[i] == true iff i has appeared */
static size_t used_cap  = 0;     /* largest index currently allocated in used */

static void
ensure_used(size_t idx)
{
    if (idx <= used_cap)
        return;
    size_t new_cap = used_cap ? used_cap * 2 : 64;
    while (new_cap < idx)
        new_cap <<= 1;
    bool *new_used = (bool *)realloc(used, (new_cap + 1) * sizeof(bool));
    if (!new_used)
        abort();                /* out of memory */
    /* initialise new entries to false */
    size_t n_new = new_cap - used_cap;
    for (size_t i = used_cap + 1; i <= new_cap; ++i)
        new_used[i] = false;
    used = new_used;
    used_cap = new_cap;
}

static void
grow_seq(size_t needed)
{
    if (needed <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *new_seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!new_seq)
        abort();                /* out of memory */
    seq = new_seq;
    seq_cap = new_cap;
}

/* main sequence entry */
seq_t a(idx_t n)
{
    if (!seq) {
        /* first call – initialise storage */
        seq_cap = 16;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq)
            abort();
        seq_len = 0;

        used_cap = 0;
        used = (bool *)malloc((used_cap + 1) * sizeof(bool));
        if (!used)
            abort();
        used[0] = false;     /* index 0 is never used */
    }

    size_t target = (size_t)n;   /* n >= 1 by definition */

    while (seq_len < target) {
        seq_t a_val = 0, b_val = 0, c_val = 0;
        bool found = false;

        /* search for the smallest unused triple */
        for (size_t ai = 1; !found; ++ai) {
            ensure_used(ai);
            if (used[ai])
                continue;   /* already used */

            seq_t a_candidate = (seq_t)ai;

            for (size_t bi = ai + 1; !found; ++bi) {
                ensure_used(bi);
                if (used[bi])
                    continue;

                seq_t b_candidate = (seq_t)bi;
                seq_t sum_ab = a_candidate + b_candidate;

                /* Compute the smallest integer s such that s^2 >= sum_ab + 1 */
                seq_t s_candidate = floor_sqrt(sum_ab + (seq_t)1);
                seq_t sq = s_candidate * s_candidate;
                if (sq < sum_ab + (seq_t)1)
                    ++s_candidate;   /* now s_candidate^2 >= sum_ab + 1 */

                /* try successive squares until an unused c > b_candidate is found */
                for (size_t si = (size_t)s_candidate; !found; ++si) {
                    seq_t square = (seq_t)si * (seq_t)si;
                    seq_t c_candidate = square - a_candidate - b_candidate;

                    if (c_candidate <= b_candidate)
                        continue;   /* enforce a < b < c */

                    /* guard against values that won't fit into size_t */
                    if (c_candidate > (seq_t)SIZE_MAX)
                        continue;
                    size_t ci = (size_t)c_candidate;

                    ensure_used(ci);
                    if (used[ci])
                        continue;   /* already used */

                    /* triple found */
                    a_val = a_candidate;
                    b_val = b_candidate;
                    c_val = c_candidate;
                    found = true;
                    break;
                }
            }
        }

        /* record the triple and mark numbers as used */
        size_t ai = (size_t)a_val;
        size_t bi = (size_t)b_val;
        size_t ci = (size_t)c_val;
        ensure_used(ai);
        ensure_used(bi);
        ensure_used(ci);
        used[ai] = true;
        used[bi] = true;
        used[ci] = true;

        /* store the triple in the sequence array */
        grow_seq(seq_len + 3);
        seq[seq_len++] = a_val;
        seq[seq_len++] = b_val;
        seq[seq_len++] = c_val;
    }

    return seq[n - 1];
}
