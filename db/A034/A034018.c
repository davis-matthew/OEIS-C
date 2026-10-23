/*
 * A034018  Numbers that are represented by x^2+xy+y^2 but not primitively.
 * https://oeis.org/A034018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by searching all integer pairs (x,y) for the form
   x^2 + x*y + y^2 and retaining values that have at least one
   representation but no primitive representation. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <stddef.h>

static seq_t *seq = NULL;          /* array of sequence values          */
static size_t seq_len = 0;         /* number of stored terms            */
static size_t seq_cap = 0;         /* allocated capacity of seq array   */
static idx_t  gen_limit = 0;       /* largest bound that has been generated */

/* Ensure that the sequence array has space for at least new_cap items. */
static void ensure_capacity(size_t new_cap)
{
    if (new_cap <= seq_cap) return;
    seq_t *new_seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq = new_seq;
    seq_cap = new_cap;
}

/* Rebuild the sequence up to (and including) the value 'limit'.  The
   previous sequence contents are discarded. */
static void rebuild_sequence(idx_t limit)
{
    /* Allocate boolean flags for representations and primitiveness */
    size_t sz = (size_t)limit + 1;
    bool *has_rep = (bool *)calloc(sz, sizeof(bool));
    bool *is_prim = (bool *)calloc(sz, sizeof(bool));
    if (!has_rep || !is_prim) {
        /* Allocation failure – abort the rebuild */
        free(has_rep);
        free(is_prim);
        return;
    }

    int64_t xmax = (int64_t)floor(sqrt((double)limit));
    for (int64_t x = -xmax; x <= xmax; ++x) {
        for (int64_t y = -xmax; y <= xmax; ++y) {
            if (x == 0 && y == 0) continue;
            /* Compute v = x^2 + x*y + y^2 safely */
            unsigned __int128 xv = (unsigned __int128)(x);
            unsigned __int128 yv = (unsigned __int128)(y);
            unsigned __int128 v128 = xv * xv + xv * yv + yv * yv;
            if (v128 == 0 || v128 > (unsigned __int128)limit) continue;
            uint64_t v = (uint64_t)v128;
            has_rep[v] = true;
            /* Primitive if gcd(|x|,|y|) == 1 */
            int64_t ax = x >= 0 ? x : -x;
            int64_t ay = y >= 0 ? y : -y;
            int64_t a = ax, b = ay;
            while (b) { int64_t t = a % b; a = b; b = t; }
            if (a == 1) is_prim[v] = true;
        }
    }

    /* Allocate new sequence array */
    size_t new_cap = 16;
    seq_t *new_seq = (seq_t *)malloc(new_cap * sizeof(seq_t));
    size_t new_len = 0;

    for (idx_t v = 1; v <= limit; ++v) {
        if (has_rep[(size_t)v] && !is_prim[(size_t)v]) {
            if (new_len == new_cap) {
                new_cap *= 2;
                seq_t *tmp = (seq_t *)realloc(new_seq, new_cap * sizeof(seq_t));
                new_seq = tmp;
            }
            new_seq[new_len++] = (seq_t)v;
        }
    }

    /* Replace old sequence with the new one */
    free(seq);
    free(has_rep);
    free(is_prim);
    seq = new_seq;
    seq_len = new_len;
    seq_cap = new_cap;
    gen_limit = limit;
}

/* Main sequence accessor */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined; return 0 for safety */

    /* Initial generation if not done yet */
    if (gen_limit == 0) rebuild_sequence(1000);

    /* Extend the sequence until we have at least n terms */
    while (seq_len < n) {
        idx_t new_limit = gen_limit * 2 + 50;  /* grow bound reasonably */
        rebuild_sequence(new_limit);
    }

    return seq[n - 1];
}
