/*
 * A033161  Begins with (2, 5); avoids 3-term arithmetic progressions.
 * https://oeis.org/A033161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A033161:  The Stanley sequence S(2,5)
    First two terms: 2, 5
    Subsequent terms are the smallest integers that keep the set free of
    3-term arithmetic progressions (greedy construction).  */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

/* Static storage for the sequence and a presence table. */
static seq_t *seq          = NULL;   /* array of terms */
static size_t seq_cap      = 0;       /* size of seq[]  */
static size_t seq_len      = 0;       /* number of terms stored */

static uint8_t *present    = NULL;   /* bit‑array: present[x] == 1 ⇔ x is in seq */
static size_t present_cap  = 0;       /* capacity of present[] */

/* Ensure seq[] has room for at least “needed” elements. */
static void ensure_seq_capacity(size_t needed) {
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 4;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    seq = tmp;
    seq_cap = new_cap;
}

/* Ensure present[] can index past “needed”. */
static void ensure_present_capacity(size_t needed) {
    if (needed < present_cap) return;
    size_t new_cap = present_cap ? present_cap << 1 : 64;
    while (new_cap <= needed) new_cap <<= 1;
    uint8_t *tmp = (uint8_t *)realloc(present, new_cap);
    if (!tmp) exit(1);
    memset(tmp + present_cap, 0, new_cap - present_cap);
    present = tmp;
    present_cap = new_cap;
}

/* Initialise the counter with the first two terms. */
static void seq_init(void) {
    if (seq_len) return;
    seq_len = 2;
    ensure_seq_capacity(2);
    seq[0] = (seq_t)2;
    seq[1] = (seq_t)5;
    ensure_present_capacity(5);
    present[2] = 1;
    present[5] = 1;
}

/* Build the sequence up to index n (1‑based). */
static void seq_build(idx_t n) {
    seq_init();
    for (idx_t i = seq_len; i < n; ++i) {
        seq_t candidate = seq[seq_len - 1] + 1;
        for (;;) {
            bool bad = false;
            uint64_t cand64 = (uint64_t)candidate;
            for (idx_t j = 0; j < seq_len; ++j) {
                uint64_t y64 = (uint64_t)seq[j];
                /* cand64 > y64 because candidate > last term. */
                uint64_t diff = cand64 - y64;           /* diff = cand - y  > 0  */
                if (diff <= y64) {                     /* cand ≤ 2·y */
                    uint64_t x64 = y64 - diff;         /* x = 2·y – cand */
                    if (x64 < present_cap && present[x64]) {
                        bad = true;
                        break;
                    }
                }
            }
            if (!bad) break;        /* candidate accepted */
            candidate++;            /* try the next integer */
        }
        /* Store the new term. */
        ensure_seq_capacity(seq_len + 1);
        seq[seq_len] = candidate;
        /* Make sure the presence table covers this value. */
        if ((uint64_t)candidate >= present_cap)
            ensure_present_capacity((uint64_t)candidate);
        present[(uint64_t)candidate] = 1;
        seq_len++;
    }
}

/* External linkage as required. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined for n = 0, return 0 */
    seq_build(n);
    return seq[n - 1];
}
