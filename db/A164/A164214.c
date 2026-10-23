/*
 * A164214  Number of binary strings of length n with equal numbers of 00010
 *          and 01000 substrings.
 * https://oeis.org/A164214
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 33, u64 n <= 65, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A164214: Number of binary strings of length n with equal numbers of
 * 00010 and 01000 substrings.  Computed by dynamic programming on a
 * 4‑bit suffix automaton that counts the difference between the two
 * patterns.  The DP ignores false matches that involve the
 * leading‑zero padding that exists before the first actual bit. */

#include <stdint.h>
#include <stdlib.h>

/* 16 possible suffixes of length 4 (0 … 15) */
#define S 16

seq_t a(idx_t n)
{
    /* The maximum absolute difference of pattern counts is at most n. */
    const int64_t offset = (int64_t)n;          /* shift index to non‑negative */
    const size_t diff_size = (size_t)(2 * offset + 1);   /* number of Δ values */
    const size_t layer_size = S * diff_size;           /* one DP layer */

    /* Allocate two layers (current & next) in one contiguous block. */
    /* calloc initializes to zero. */
    seq_t *dp = (seq_t *)calloc(2 * layer_size, sizeof(seq_t));
    if (dp == NULL) return (seq_t)0;          /* safe fallback on OOM */

    seq_t *cur  = dp;
    seq_t *next = dp + layer_size;

    /* Initial state: empty string – last 4 bits are all zeros and Δ = 0. */
    cur[0 * diff_size + offset] = (seq_t)1;

    /* Main DP loop: process each of the n bits. */
    for (idx_t step = 0; step < n; ++step) {
        /* Clear the next layer. */
        for (size_t i = 0; i < layer_size; ++i) next[i] = (seq_t)0;

        /* At 'step' we have processed 'step' bits; reachable |d| <= step. */
        int64_t cur_range = (int64_t)step;

        for (int s = 0; s < S; ++s) {
            for (int64_t d = -cur_range; d <= cur_range; ++d) {
                size_t idx_cur = (size_t)s * diff_size + (size_t)(d + offset);
                seq_t val = cur[idx_cur];
                if (val == (seq_t)0) continue;

                /* Append each possible bit. */
                for (int bit = 0; bit < 2; ++bit) {
                    int newstate = ((s << 1) | bit) & 0xF;          /* last 4 bits */
                    int new5    = ((s << 1) | bit) & 0x1F;          /* last 5 bits */

                    /* Only count a pattern when we have at least 5 real bits
                     * (i.e., after the first 4 bits have been processed). */
                    int inc = 0;
                    if (step >= 4) {
                        if (new5 == 0b00010) inc = 1;   /* 00010 increases Δ */
                        else if (new5 == 0b01000) inc = -1; /* 01000 decreases Δ */
                    }

                    int64_t nd = d + inc;
                    size_t idx_next = (size_t)newstate * diff_size + (size_t)(nd + offset);
                    next[idx_next] += val;
                }
            }
        }

        /* Swap current and next layers for the next iteration. */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
    }

    /* Sum over all states where the difference Δ == 0. */
    seq_t result = (seq_t)0;
    for (int s = 0; s < S; ++s) {
        size_t idx = (size_t)s * diff_size + offset;
        result += cur[idx];
    }

    free(dp);
    return result;
}
