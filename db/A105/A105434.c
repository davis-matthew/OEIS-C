/*
 * A105434  a(1)=1. a(n+1) = a(n) + (number of terms, from among the first n
 *          terms of the sequence, which occur among
 *          (d(a(1)),d(a(2)),d(a(3)),...d(a(n))), where d(n) = A000005(n)).
 * https://oeis.org/A105434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(n+1)=a(n)+|{m≤n : a(m)∈{d(a(1)),…,d(a(n))}|, where
 *            d(k)=number of divisors of k
 * Range:     u8 n <= 47, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   divisor_count, seq_contains
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation keeps all previously computed terms in a
 * static 1‑based array `seq`.  For every n it also remembers the set
 * `S` of divisor counts that have appeared so far.  The variable
 * `inter` holds the current value of the set intersection:
 * the number of terms among the first n terms that belong to S.
 * When a new term is generated the intersection is updated
 * incrementally.  Divisor counts are computed by a small trial‑division
 * routine that works for both 64‑bit integers and for the arbitrary‑
 * precision build (the result always fits in seq_t). */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/divisor_count.h"
#include "../utils/seq_contains.h"

/* Forward declaration for the divisor‑count helper. */
static seq_t divisor_count(seq_t x);

seq_t a(idx_t n)
{
    /* a(1) is the first term; a(0) is unused. */
    if (n == 0)
        return (seq_t)0;

    /* Static state, preserved between calls. */
    static idx_t current_n = 0;          /* number of terms stored */
    static seq_t *seq = NULL;           /* 1‑based array of terms */
    static size_t seq_cap = 0;          /* capacity of seq array */
    static uint8_t *Sbool = NULL;       /* S[d]=1 iff divisor count d has appeared */
    static size_t S_cap = 0;            /* capacity of Sbool array */
    static unsigned inter = 0;          /* intersection size for current_n */

    /* First call: initialise. */
    if (current_n == 0) {
        /* Allocate space for sequence. */
        seq_cap = 4;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq[1] = (seq_t)1;            /* a(1)=1 */
        current_n = 1;

        /* Allocate space for divisor‐count set. */
        S_cap = 4;
        Sbool = (uint8_t *)calloc(S_cap, sizeof(uint8_t));
        Sbool[1] = 1;                  /* d(1)=1 present */
        inter = 1;                     /* {1}∩{1} has size 1 */
    }

    /* Grow state until it contains at least n terms. */
    while (current_n < n) {
        /* Generate a(new) = a(old) + intersection. */
        seq_t last = seq[current_n];
        seq_t new_val = last + (seq_t)inter;

        /* Divisor count of the new term. */
        seq_t d_new = divisor_count(new_val);
        size_t d_idx = (size_t)d_new;

        /* Grow Sbool to accommodate d_idx. */
        if (d_idx >= S_cap) {
            size_t new_cap = S_cap * 2;
            if (new_cap == 0) new_cap = 4;
            while (new_cap <= d_idx) new_cap <<= 1;
            uint8_t *tmp = (uint8_t *)realloc(Sbool, new_cap * sizeof(uint8_t));
            if (!tmp) abort();
            memset(tmp + S_cap, 0, (new_cap - S_cap) * sizeof(uint8_t));
            Sbool = tmp;
            S_cap = new_cap;
        }

        unsigned delta = 0;
        bool prev_has_d = Sbool[d_idx];

        /* If d_new is new, add it to S. */
        if (!prev_has_d) {
            Sbool[d_idx] = 1;
            /* Terms equal to d_new in the existing sequence become counted. */
            if (seq_contains(seq, current_n, d_new))
                delta++;
        }

        /* The new term itself may now belong to S. */
        if (new_val < S_cap && Sbool[(size_t)new_val])
            delta++;

        inter += delta;

        /* Grow seq array if needed. */
        if (current_n + 1 >= seq_cap) {
            size_t new_cap = seq_cap * 2;
            if (new_cap <= current_n + 1) new_cap = current_n + 2;
            seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            seq = tmp;
            seq_cap = new_cap;
        }

        seq[++current_n] = new_val;
    }

    return seq[n];
}
