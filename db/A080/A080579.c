/*
 * A080579  a(1)=1; for n>1, a(n)=a(n-1)+1 if n is already in the sequence,
 *          a(n)=a(n-1)+4 otherwise.
 * https://oeis.org/A080579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recursive definition: a(1)=1; for n>1, a(n)=a(n-1)+1 if n is already in the sequence,
   otherwise a(n)=a(n-1)+4. */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static seq_t *seq        = NULL;
static size_t seq_len    = 0;
static size_t seq_cap    = 0;

static unsigned char *present = NULL;
static size_t present_cap     = 0;

static void ensure_seq_capacity(size_t needed) {
    if (needed <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap : 1;
    while (new_cap < needed)
        new_cap <<= 1;
    seq = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

static void ensure_present_capacity(size_t needed) {
    if (needed < present_cap)
        return;
    size_t new_cap = present_cap ? present_cap : 1;
    while (new_cap <= needed)
        new_cap <<= 1;
    unsigned char *new_arr = (unsigned char*)realloc(present, new_cap * sizeof(unsigned char));
    /* Zero out the newly allocated part */
    memset(new_arr + present_cap, 0, new_cap - present_cap);
    present = new_arr;
    present_cap = new_cap;
}

seq_t a(idx_t n) {
    /* Initialise the sequence on first call */
    if (seq_len == 0) {
        ensure_seq_capacity(1);
        seq[0] = (seq_t)1;
        seq_len = 1;
        ensure_present_capacity(1);
        present[1] = 1;
    }

    /* Fast return if already computed */
    if ((size_t)n <= seq_len)
        return seq[(size_t)n - 1];

    /* Compute values up to n */
    for (size_t i = seq_len + 1; i <= (size_t)n; ++i) {
        bool seen = false;
        if (i < present_cap)
            seen = present[i];
        seq_t delta = seen ? (seq_t)1 : (seq_t)4;
        seq_t val = seq[seq_len - 1] + delta;

        ensure_seq_capacity(seq_len + 1);
        seq[seq_len] = val;
        seq_len++;

        /* Mark the new value as seen */
        ensure_present_capacity((size_t)val);
        present[(size_t)val] = 1;
    }

    return seq[(size_t)n - 1];
}
