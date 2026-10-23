/*
 * A137838  Limiting sequence when we start with the positive integers
 *          (A000027) and delete in step n >= 1 the first digit in the term at
 *          position n + a(n).
 * https://oeis.org/A137838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) computed by the iterative deletion process defined in OEIS
 *            A137838
 * Range:     u8 n <= 26, u16 n <= 26, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   digit_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/digit_len.h"

/* Static mutable state used by the sequence function */
static seq_t *seq = NULL;               /* current sequence values */
static size_t seq_len = 0, seq_cap = 0;

static seq_t *memo = NULL;              /* cached values of a(n) */
static size_t memo_len = 0, memo_cap = 0;

static idx_t last_step = 0;             /* number of steps already processed */
static unsigned long long next_val = 1; /* next integer to append when extending seq */

/* Ensure seq contains at least target elements, extending with consecutive integers */
static void ensure_seq_size(size_t target) {
    if (seq_len >= target) return;
    if (seq_cap < target) seq_cap = target;
    seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
    for (size_t i = seq_len; i < target; ++i) {
        seq[i] = (seq_t)next_val++;
    }
    seq_len = target;
}

/* Ensure memo contains up to needed elements */
static void ensure_memo_size(size_t needed) {
    if (memo_len >= needed) return;
    if (memo_cap < needed) memo_cap = needed;
    memo = (seq_t *)realloc(memo, memo_cap * sizeof(seq_t));
    memo_len = needed;
}

/* Perform a single step of the process (step number k, 1‑indexed) */
static void process_step(size_t k) {
    if (seq_len < k) ensure_seq_size(k);
    seq_t a_k = seq[k-1];

    ensure_memo_size(k);
    memo[k-1] = a_k;

    size_t pos = k + (size_t)a_k;          /* 1‑indexed position to modify */
    if (seq_len < pos) ensure_seq_size(pos);
    seq_t val = seq[pos-1];

    if (val >= 10) {
        size_t d = digit_len(val) - 1;     /* number of remaining digits */
        seq_t pow10 = 1;
        for (size_t i = 0; i < d; ++i) pow10 *= 10;
        seq[pos-1] = val % pow10;          /* remove first digit */
    } else {
        /* Remove the element completely */
        for (size_t idx = pos-1; idx < seq_len-1; ++idx)
            seq[idx] = seq[idx+1];
        seq_len -= 1;
    }
}

/* Main function required by the build system */
seq_t a(idx_t n) {
    /* Advance the process up to step n‑1 (all deletions up to that point) */
    while (last_step < n-1) {
        process_step((size_t)(last_step + 1));
        last_step++;
    }

    /* Ensure the sequence contains the n‑th element */
    if (seq_len < (size_t)n) ensure_seq_size((size_t)n);

    /* Store a(n) if not already cached and return it */
    if ((size_t)n > memo_len) {
        seq_t val = seq[(size_t)n - 1];
        ensure_memo_size((size_t)n);
        memo[(size_t)n - 1] = val;
        return val;
    }
    return memo[(size_t)n - 1];
}
