/*
 * A118374  Lexicographically earliest positive integer sequence no two terms
 *          of which sum to a term of {1,7,23,63,159,...} = {n*2^n-1},
 *          n=1,2,3,... The first differences are given in A119350.
 * https://oeis.org/A118374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A118374 – lexicographically earliest sequence with no pairwise sum of the form
 * k*2^k-1.
 * Recurrence: a(1)=1; for n>1, a(n) is the smallest integer >a(n-1) such that
 * a(n)+a(i) ∉ {k*2^k-1 | k≥1} for all i<n.  The function builds the sequence
 * lazily and memoises it so that arbitrary order of calls is supported. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Forward declarations for utilities that may be used for heap sizes, etc. */
static seq_t *seq = NULL;            /* memoised sequence values               */
static size_t seq_size = 0, seq_cap = 0;

/* Forbidden sums k*2^k-1, kept sorted. */
static seq_t *forbidden = NULL;
static size_t forb_size = 0, forb_cap = 0;
static size_t next_k = 1;            /* next k to generate for forbidden list  */

static bool init_done = false;

/* Ensure seq array has space for at least 'needed' elements. */
static void ensure_seq_cap(size_t needed) {
    if (needed <= seq_cap) return;
    size_t newcap = seq_cap ? seq_cap << 1 : 16;
    while (newcap < needed) newcap <<= 1;
    seq = (seq_t *)realloc(seq, newcap * sizeof(seq_t));
    seq_cap = newcap;
}

/* Ensure forbidden array contains all values f = k*2^k-1 with f <= limit. */
static void ensure_forbidden_upto(seq_t limit) {
    while (forb_size == 0 || forbidden[forb_size - 1] <= limit) {
        seq_t pow2 = (seq_t)1;
        /* Multiply pow2 by 2, 'next_k' times. */
        for (size_t i = 0; i < next_k; ++i) pow2 *= (seq_t)2;
        seq_t f = ((seq_t)next_k) * pow2 - (seq_t)1;

        if (forb_cap <= forb_size) {
            forb_cap = forb_cap ? forb_cap << 1 : 16;
            forbidden = (seq_t *)realloc(forbidden, forb_cap * sizeof(seq_t));
        }
        forbidden[forb_size++] = f;

        if (f > limit) break;   /* we have covered up to 'limit'           */
        ++next_k;
    }
}

/* Binary search in forbidden array. Assuming forbidden is sorted. */
static bool forbidden_sum(seq_t s) {
    size_t lo = 0, hi = forb_size;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (forbidden[mid] < s)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (lo < forb_size && forbidden[lo] == s);
}

/* Initialise data structures lazily. */
static void init(void) {
    if (init_done) return;
    /* initialise sequence storage */
    seq_cap = 16;
    seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    seq_size = 0;
    /* initialise forbidden storage */
    forb_cap = 16;
    forbidden = (seq_t *)malloc(forb_cap * sizeof(seq_t));
    forb_size = 0;
    next_k = 1;
    init_done = true;
}

/* Public function implementing the sequence. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* undefined for offset 1, safe guard   */
    init();

    /* Generate terms up to n if needed. */
    while (seq_size < (size_t)n) {
        seq_t candidate;
        if (seq_size == 0)
            candidate = (seq_t)1;          /* first term                            */
        else
            candidate = seq[seq_size - 1] + (seq_t)1;

        /* Ensure forbidden numbers up to the maximum possible sum for this step. */
        /* The largest possible sum after adding this candidate is */
        /* candidate + previous candidate.  It suffices to ensure the forbidden list
         * goes beyond that. */
        seq_t max_needed = candidate + seq[seq_size - 1];
        ensure_forbidden_upto(max_needed);

        /* Find the smallest candidate satisfying the condition. */
        for (;;) {
            bool ok = true;
            for (size_t i = 0; i < seq_size; ++i) {
                seq_t sum = candidate + seq[i];
                if (forbidden_sum(sum)) {
                    ok = false;
                    break;
                }
            }
            if (ok) break;
            ++candidate;
        }

        /* Store the found term. */
        ensure_seq_cap(seq_size + 1);
        seq[seq_size++] = candidate;
    }

    return seq[n - 1];
}
