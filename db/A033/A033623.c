/*
 * A033623  Grundy function for turn-at-most-4-coins game.
 * https://oeis.org/A033623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = smallest natural not the exclusive-or of at most three
 *            a(k) for k < n
 * Range:     u8 n <= 17, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Sequence storage */
static seq_t *seq = NULL;
static idx_t seq_len = 0;
static size_t seq_cap = 0;

/* Bitsets for fast look‑up */
static unsigned char *seen_single = NULL;
static size_t seen_cap = 0;
static unsigned char *seen_pair   = NULL;
static size_t pair_cap   = 0;

/* Next candidate for the mex */
static idx_t next_candidate = 0;

/* Ensure the single‑value bitset can hold 'val' */
static inline void ensure_seen_single(idx_t val) {
    if (val < seen_cap) return;
    size_t new_cap = seen_cap ? seen_cap << 1 : 64;
    while (new_cap <= (size_t)val) new_cap <<= 1;
    unsigned char *tmp = (unsigned char*)realloc(seen_single, new_cap);
    if (!tmp) exit(1);
    memset(tmp + seen_cap, 0, new_cap - seen_cap);
    seen_single = tmp;
    seen_cap = new_cap;
}

/* Ensure the pair‑xor bitset can hold 'val' */
static inline void ensure_seen_pair(idx_t val) {
    if (val < pair_cap) return;
    size_t new_cap = pair_cap ? pair_cap << 1 : 64;
    while (new_cap <= (size_t)val) new_cap <<= 1;
    unsigned char *tmp = (unsigned char*)realloc(seen_pair, new_cap);
    if (!tmp) exit(1);
    memset(tmp + pair_cap, 0, new_cap - pair_cap);
    seen_pair = tmp;
    pair_cap = new_cap;
}

/* Test whether a value is representable by XOR of up to three prior terms */
static inline bool is_representable(seq_t x, idx_t current_len) {
    if ((seq_t)0 == x) return true;               /* XOR of zero terms is 0 */
    idx_t xv = (idx_t)x;
    if (xv < seen_cap && seen_single[xv]) return true;
    if (xv < pair_cap && seen_pair[xv])    return true;
    for (idx_t i = 0; i < current_len; ++i) {
        seq_t tmp = x ^ seq[i];
        idx_t tv = (idx_t)tmp;
        if (tv < pair_cap && seen_pair[tv]) return true;
    }
    return false;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;  /* Undefined for 0, but harmless */
    while (seq_len < n) {
        /* Find the mex */
        seq_t val;
        for (;;) {
            seq_t cand_val = (seq_t)next_candidate;
            if (!is_representable(cand_val, seq_len)) {
                val = cand_val;
                break;
            }
            ++next_candidate;
        }

        /* Grow storage if necessary */
        if (seq_len == seq_cap) {
            seq_cap = seq_cap ? seq_cap << 1 : 8;
            seq_t *tmp = (seq_t*)realloc(seq, seq_cap * sizeof(seq_t));
            if (!tmp) exit(1);
            seq = tmp;
        }
        seq[seq_len] = val;

        /* Mark this value as a singleton */
        ensure_seen_single((idx_t)val);
        seen_single[(idx_t)val] = 1;

        /* Mark all XORs of this value with previous values */
        for (idx_t i = 0; i < seq_len; ++i) {
            seq_t pair_val = val ^ seq[i];
            ensure_seen_pair((idx_t)pair_val);
            seen_pair[(idx_t)pair_val] = 1;
        }

        ++seq_len;
        ++next_candidate;   /* Next mex search starts just after the new value */
    }
    return seq[n - 1];
}
