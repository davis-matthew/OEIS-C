/*
 * A103708  a(n+1) = d(n)+9 where d(n) is the n-th decimal digit in the
 *          concatenation of the digits in the sequence, starting with a(1)=1.
 * https://oeis.org/A103708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n+1) = d(n)+9 where d(n) is the nth decimal digit in the
 *            concatenation of the terms of the sequence, with a(1)=1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Static storage for computed terms and concatenated digits */
static seq_t *seq_arr = NULL;       /* sequence values */
static size_t seq_cap = 0;          /* allocated capacity for seq_arr */
static idx_t seq_len = 0;           /* number of terms stored (>=0) */

static unsigned char *digit_buf = NULL; /* concatenated digits */
static size_t digit_cap = 0;            /* allocated capacity for digit_buf */
static size_t digit_len = 0;            /* number of digits stored */

/* Ensure seq_arr has at least min_cap elements allocated */
static void seq_ensure_cap(size_t min_cap) {
    if (seq_cap >= min_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    if (new_cap < min_cap) new_cap = min_cap;
    seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_arr) { abort(); }
    if (seq_arr) memcpy(new_arr, seq_arr, seq_len * sizeof(seq_t));
    free(seq_arr);
    seq_arr = new_arr;
    seq_cap = new_cap;
}

/* Ensure digit_buf has at least min_cap elements allocated */
static void digit_ensure_cap(size_t min_cap) {
    if (digit_cap >= min_cap) return;
    size_t new_cap = digit_cap ? digit_cap * 2 : 32;
    if (new_cap < min_cap) new_cap = min_cap;
    unsigned char *new_buf = (unsigned char *)malloc(new_cap * sizeof(unsigned char));
    if (!new_buf) { abort(); }
    if (digit_buf) memcpy(new_buf, digit_buf, digit_len * sizeof(unsigned char));
    free(digit_buf);
    digit_buf = new_buf;
    digit_cap = new_cap;
}

/* Append a single digit (0-9) to the digit buffer */
static void digit_append(unsigned char d) {
    digit_ensure_cap(digit_len + 1);
    digit_buf[digit_len++] = d;
}

/* Append digits of a two‑digit number to the digit buffer */
static void digit_append_two(seq_t val) {
    /* val is assumed to be between 10 and 18 */
    digit_ensure_cap(digit_len + 2);
    digit_buf[digit_len++] = (unsigned char)(val / 10);
    digit_buf[digit_len++] = (unsigned char)(val % 10);
}

/* Helper to initialize the first term (a(1)=1) */
static void initialize(void) {
    seq_ensure_cap(1);
    seq_arr[0] = 1;
    seq_len = 1;

    digit_ensure_cap(1);
    digit_buf[0] = 1;     /* digit of a(1) */
    digit_len = 1;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0;  /* undefined for n<1, return 0 */

    if (seq_len == 0) initialize();

    /* Compute terms up to n */
    while (seq_len < n) {
        /* a(k+1) depends on d(k), where k = seq_len */
        unsigned char digit_k = digit_buf[seq_len - 1];  /* d(k) */
        seq_t next = (seq_t)(digit_k + 9);

        /* Append digits of next term to the buffer */
        if (next >= 10) {
            digit_append_two(next);
        } else {
            digit_append((unsigned char)next);
        }

        /* Store the new term */
        seq_ensure_cap(seq_len + 1);
        seq_arr[seq_len++] = next;
    }

    return seq_arr[n - 1];
}
