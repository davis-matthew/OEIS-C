/*
 * A026272  a(n) = smallest k such that k=a(n-k-1) is the only appearance of k
 *          so far; if there is no such k, then a(n) = least positive integer
 *          that has not yet appeared.
 * https://oeis.org/A026272
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 412, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the recurrence for OEIS A026272  
   a(n) = the smallest k such that k = a(n-k-1) and k has appeared exactly once so far;  
   if no such k exists, a(n) is the least positive integer not yet seen. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Dynamic array storing computed terms (1‑based indices mapped to 0‑based slots) */
static seq_t *seq      = NULL;
static size_t seq_len  = 0;   /* number of terms currently stored */
static size_t seq_cap  = 0;   /* allocated capacity of seq[] */

/* Counters and last positions for values that have appeared */
static uint8_t   *value_count = NULL;   /* 0, 1, or 2 occurrences */
static idx_t    *value_last  = NULL;   /* last index where value occurred */
static idx_t     val_cap      = 0;      /* allocated capacity (max value index + 1) */

/* Smallest positive integer not yet seen */
static idx_t next_unused = 1;

/* Ensure the sequence array can hold at least 'needed' elements */
static void ensure_seq_cap(idx_t needed) {
    if (seq_cap >= needed) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 16;
    while (new_cap < (size_t)needed) new_cap <<= 1;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Ensure the value bookkeeping arrays can hold up to 'max_value' */
static void ensure_val_cap(idx_t max_value) {
    if (max_value <= val_cap) return;
    size_t new_cap = val_cap ? val_cap << 1 : 32;
    while (new_cap < (size_t)max_value + 1) new_cap <<= 1;
    uint8_t *cnt_new  = (uint8_t *)realloc(value_count, new_cap * sizeof(uint8_t));
    idx_t   *last_new = (idx_t *)realloc(value_last,   new_cap * sizeof(idx_t));
    if (!cnt_new || !last_new) exit(EXIT_FAILURE);
    /* initialise new entries to zero */
    memset(cnt_new + val_cap, 0, (new_cap - val_cap) * sizeof(uint8_t));
    /* value_last entries are left uninitialised (0) which is fine for unused values */
    value_count = cnt_new;
    value_last  = last_new;
    val_cap = new_cap;
}

/* Compute all terms up to index n (1‑based) */
static void compute_to(idx_t n) {
    if (n <= seq_len) return;          /* already computed */
    for (idx_t pos = seq_len + 1; pos <= n; ++pos) {
        /* Guarantee enough bookkeeping space for the likely value range */
        ensure_val_cap(pos);

        seq_t val = 0;
        bool found = false;

        /* Search for the smallest k satisfying the defining condition */
        for (idx_t k = 1; k < pos; ++k) {
            if (value_last[k] == pos - k - 1 && value_count[k] == 1) {
                val = (seq_t)k;
                found = true;
                break;
            }
        }

        if (!found) {
            /* Pick the smallest unused integer */
            while (next_unused <= val_cap && value_count[next_unused] != 0) {
                ++next_unused;
            }
            if (next_unused > val_cap) ensure_val_cap(next_unused);
            val = (seq_t)next_unused;
        }

        /* Ensure we can index the value in our bookkeeping arrays */
        idx_t v = (idx_t)val;
        ensure_val_cap(v);

        /* Store the term */
        ensure_seq_cap(pos);
        seq[seq_len++] = val;

        /* Update bookkeeping for this value */
        value_count[v] += 1;
        value_last[v]  = pos;

        /* Advance the least unused integer if it matches the value just used */
        if (v == next_unused) {
            do { ++next_unused; }
            while (next_unused <= val_cap && value_count[next_unused] != 0);
        }
    }
}

/* External function required by the specification */
seq_t a(idx_t n) {
    if (n == 0) return 0;  /* undefined for n=0, return 0 as safety */
    compute_to(n);
    return seq[n - 1];
}
