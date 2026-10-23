/*
 * A182045  Lexicographically earliest sequence: a(n)+a(n+1) is alternatively
 *          square and cube, a(1)=1.
 * https://oeis.org/A182045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_cube_root, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Lexicographically earliest sequence where a(n)+a(n+1) alternates between
   perfect squares (odd n) and perfect cubes (even n).  The algorithm
   greedily chooses the smallest unused positive integer that satisfies the
   required sum for each step. */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/floor_sqrt.h"
#include "../utils/floor_cube_root.h"

#ifdef __cplusplus
#include <vector>
#endif

/*===================================================================*/
/*  C++ implementation: use std::vector for storage                   */
/*===================================================================*/
#ifdef __cplusplus
static std::vector<seq_t> seq_vals;      /* sequence a(1)..a(k)   */
static std::vector<seq_t> used_vals;    /* set of used values    */
#else
/*===================================================================*/
/*  C implementation: use dynamic arrays                             */
/*===================================================================*/
static seq_t *seq_vals = NULL;   /* sequence a(1)..a(k)   */
static size_t seq_len  = 0;      /* current length        */
static size_t seq_cap  = 0;      /* allocated capacity    */

static seq_t *used_vals = NULL;  /* set of used values    */
static size_t used_len  = 0;      /* current used count    */
static size_t used_cap  = 0;      /* allocated capacity    */

/* Ensure seq_vals has space for at least one more element            */
static void ensure_seq_cap(void) {
    if (seq_len == seq_cap) {
        seq_cap = seq_cap ? seq_cap * 2 : 16;
        seq_vals = (seq_t *)realloc(seq_vals, seq_cap * sizeof(seq_t));
    }
}

/* Ensure used_vals has space for at least one more element            */
static void ensure_used_cap(void) {
    if (used_len == used_cap) {
        used_cap = used_cap ? used_cap * 2 : 16;
        used_vals = (seq_t *)realloc(used_vals, used_cap * sizeof(seq_t));
    }
}
#endif

/*-------------------------------------------------------------------*/
/*  Test whether a value has already appeared in the sequence         */
/*-------------------------------------------------------------------*/
static bool value_used(seq_t v) {
#ifdef __cplusplus
    for (size_t i = 0; i < used_vals.size(); ++i)
        if (used_vals[i] == v) return true;
    return false;
#else
    for (size_t i = 0; i < used_len; ++i)
        if (used_vals[i] == v) return true;
    return false;
#endif
}

/*===================================================================*/
/*  Main sequence function                                           */
/*===================================================================*/
seq_t a(idx_t n)
{
    if (n == 0)   /* OEIS offset 1; return 0 for n==0 (never used) */
        return 0;

    /* initialise the sequence with a(1)=1 if empty */
#ifdef __cplusplus
    if (seq_vals.empty()) {
        seq_vals.push_back(1);
        used_vals.push_back(1);
    }
#else
    if (seq_len == 0) {
        ensure_seq_cap();
        seq_vals[seq_len++] = 1;
        ensure_used_cap();
        used_vals[used_len++] = 1;
    }
#endif

    /* grow the sequence up to the requested index */
#ifdef __cplusplus
    while (seq_vals.size() < (size_t)n) {
        seq_t cur = seq_vals.back();                     /* a(i)   */
        bool need_square = (seq_vals.size() % 2 == 1);   /* odd i  */

        seq_t t = need_square
                    ? floor_sqrt(cur) + 1
                    : floor_cube_root(cur) + 1;

        seq_t target, cand;
        for (;;) {
            target = need_square ? t * t : t * t * t;
            cand   = target - cur;                       /* a(i+1) */
            if (cand > 0 && !value_used(cand))
                break;
            ++t;
        }
        seq_vals.push_back(cand);
        used_vals.push_back(cand);
    }
    return seq_vals[n - 1];
#else
    while (seq_len < (size_t)n) {
        seq_t cur = seq_vals[seq_len - 1];
        bool need_square = (seq_len % 2 == 1);

        seq_t t = need_square
                    ? floor_sqrt(cur) + 1
                    : floor_cube_root(cur) + 1;

        seq_t target, cand;
        for (;;) {
            target = need_square ? t * t : t * t * t;
            cand   = target - cur;  /* a(i+1) */
            if (cand > 0 && !value_used(cand))
                break;
            ++t;
        }
        ensure_seq_cap();
        seq_vals[seq_len++] = cand;
        ensure_used_cap();
        used_vals[used_len++] = cand;
    }
    return seq_vals[n - 1];
#endif
}
