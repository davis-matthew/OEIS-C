/*
 * A045262  Numbers whose base-5 representation contains exactly three 1's and
 *          three 4's.
 * https://oeis.org/A045262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth smallest integer whose base‑5 representation
 *            contains exactly three 1’s and three 4’s. This implementation
 *            generates the values in ascending order by scanning all numbers
 *            of gradually increasing base‑5 length and selecting those that
 *            satisfy the digit constraint
 * Range:     u8 none, u16 n <= 20, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *seq_values = NULL;
static size_t seq_len   = 0;
static size_t seq_cap   = 0;
static idx_t next_len   = 6;    /* next base‑5 length to generate */
static int  pow_start_initialized = 0;
static seq_t pow_start;         /* 5^(next_len-1) */

/* Append a value to the dynamic sequence array. */
static void seq_push(seq_t val) {
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap << 1 : 1024;
        seq_t *tmp = (seq_t *)realloc(seq_values, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* memory failure */
        seq_values = tmp;
        seq_cap = new_cap;
    }
    seq_values[seq_len++] = val;
}

/* Generate all numbers of base‑5 length == next_len that contain
   exactly three 1’s and three 4’s, and store them in ascending order. */
static void generate_current_length(void) {
    if (!pow_start_initialized) {
        pow_start = 1;
        for (idx_t i = 0; i < next_len - 1; ++i) pow_start *= 5;
        pow_start_initialized = 1;
    }

    seq_t start = pow_start;           /* first number of this length */
    seq_t end   = start * 5 - 1;        /* largest number of this length */

    for (seq_t val = start; val <= end; ++val) {
        int cnt1 = 0, cnt4 = 0;
        seq_t tmp = val;
        for (idx_t p = 0; p < next_len; ++p) {
            seq_t d = tmp % 5;
            if (d == 1) ++cnt1;
            else if (d == 4) ++cnt4;
            tmp /= 5;
        }
        if (cnt1 == 3 && cnt4 == 3) seq_push(val);
    }

    /* Advance to the next length. */
    next_len += 1;
    pow_start *= 5;
}

/* Ensure the sequence contains at least n terms. */
static void ensure_sequence(idx_t n) {
    while (seq_len < n) {
        generate_current_length();
    }
}

/* Public function to return a(n). */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    ensure_sequence(n);
    if ((size_t)n > seq_len) return (seq_t)0;   /* safety guard */
    return seq_values[n - 1];
}
