/*
 * A043785  Numbers n such that number of runs in the base 3 representation of
 *          n is congruent to 0 mod 6.
 * https://oeis.org/A043785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th integer whose base‑3 representation has a
 * number of runs that is congruent to 0 mod 6.  The sequence is built
 * incrementally by testing integers in ascending order. */
#ifndef __cplusplus
#include <stdbool.h>
#include <stdlib.h>
#endif

/* Helper: test whether the base‑3 representation of v has a number of
 * runs divisible by 6. */
static bool runs_mod6_zero(seq_t v) {
    if (v == 0)
        return false;          /* 0 has one run → 1 mod 6 */
    seq_t x = v;
    int prev = (int)(x % 3);
    int runs = 1;
    x /= 3;
    while (x > 0) {
        int d = (int)(x % 3);
        if (d != prev) {
            ++runs;
            prev = d;
        }
        x /= 3;
    }
    return (runs % 6) == 0;
}

#ifdef __cplusplus
/* C++17 build: use std::vector for automatic construction. */
#include <vector>
using std::vector;

static vector<seq_t> seq;
static seq_t next_candidate = 1;
#else
/* C11 build: manual dynamic array. */
static seq_t *seq = NULL;
static size_t seq_size = 0;
static size_t seq_cap = 0;
static seq_t next_candidate = 1;
#endif

seq_t a(idx_t n) {
    /* Ensure we have computed up to index n. */
#ifdef __cplusplus
    while (seq.size() < n) {
        if (runs_mod6_zero(next_candidate))
            seq.push_back(next_candidate);
        ++next_candidate;
    }
    return seq[n - 1];
#else
    if (seq == NULL) {
        seq_cap = 16;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq_size = 0;
    }
    while (seq_size < (size_t)n) {
        if (runs_mod6_zero(next_candidate)) {
            if (seq_size == seq_cap) {
                seq_cap = seq_cap ? seq_cap * 2 : 16;
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_size++] = next_candidate;
        }
        ++next_candidate;
    }
    return seq[n - 1];
#endif
}
