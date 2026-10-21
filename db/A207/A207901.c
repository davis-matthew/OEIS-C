/*
 * A207901  Let S_k denote the first 2^k terms of this sequence and let b_k be
 *          the smallest positive integer that is not in S_k, also let R_k
 *          equal S_k read in reverse order; then the numbers b_k*R_k are the
 *          next 2^k terms.
 * https://oeis.org/A207901
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 84, u32 n <= 1364, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A207901: recursively build a permutation of the positive integers.
 *  Start with S_0 = {1}.  For k ≥ 0 let b_k be the smallest positive integer
 *  not in S_k, R_k the reverse of S_k, and append b_k * R_k to S_k to obtain
 *  S_{k+1}.  The resulting sequence a(n) is the n‑th element of S_k
 *  for any n (0‑based). */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Static storage for the generated terms. */
static seq_t *seq           = NULL;
static size_t seq_len       = 0;   /* number of terms stored */
static size_t seq_cap       = 0;   /* array capacity */

/* Ensure the array can hold at least 'min_cap' elements. */
static void ensure_seq_cap(size_t min_cap) {
    if (seq_cap >= min_cap) return;
    size_t cap = seq_cap ? seq_cap << 1 : 4;
    while (cap < min_cap) cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq, cap * sizeof(seq_t));
    seq = tmp;
    seq_cap = cap;
}

/* Append a single term to the array. */
static void append_term(seq_t val) {
    ensure_seq_cap(seq_len + 1);
    seq[seq_len++] = val;
}

/* Find the smallest positive integer not present in the current sequence. */
static seq_t find_missing(void) {
    seq_t cand = 1;
    while (true) {
        bool present = false;
        for (size_t i = 0; i < seq_len; ++i) {
            if (seq[i] == cand) { present = true; break; }
        }
        if (!present) return cand;
        ++cand;
    }
}

/* Expand the generated sequence up to at least index n (0‑based). */
static void ensure_sequence_up_to(idx_t n) {
    /* Initial condition: a(0) = 1 */
    if (seq_len == 0) {
        append_term((seq_t)1);
    }

    while ((idx_t)seq_len <= n) {
        /* Current block size = 2^k = seq_len */
        size_t block  = seq_len;
        seq_t missing = find_missing();

        /* Append block elements: missing * reversed(block elements) */
        for (idx_t i = (idx_t)block; i > 0; --i) {
            seq_t val = missing * seq[i - 1];
            append_term(val);
        }
    }
}

/* Public entry point required by the test harness. */
seq_t a(idx_t n) {
    ensure_sequence_up_to(n);
    return seq[n];
}
