/*
 * A206257  Values of S(1) such that any Mersenne prime with an odd exponent p
 *          divides S(p-2), where S(n) == S(n-1)^2 - 2 (mod M(p)).
 * https://oeis.org/A206257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A206257:
   a(n) = sorted merge of two recurrences.
   Recurrence A: A0 = 14, A1 = 2702,  A(k) = 194*A(k-1) - A(k-2)
   Recurrence B: B0 = 98,  B1 = 940898, B(k) = 9602*B(k-1) - B(k-2)
   The merged sequence is returned in strictly ascending order.  */

/* Required utility for dynamic array growth */
#include "../utils/ensure_capacity.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *a_buf = NULL;     /* Values of recurrence A */
static seq_t *b_buf = NULL;     /* Values of recurrence B */
static seq_t *merged = NULL;    /* Sorted merge of a_buf and b_buf */

static size_t a_cap = 0, a_len = 0, a_out = 0;
static size_t b_cap = 0, b_len = 0, b_out = 0;
static size_t merged_cap = 0, merged_len = 0;

/* Helper that reallocates a buffer to at least 'needed' elements. */
static inline void ensure_cap(size_t needed, size_t *cap, seq_t **buf) {
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 16 : *cap * 2;
    while (new_cap < needed) new_cap *= 2;
    ensure_capacity(new_cap, cap, (void **)buf, sizeof(seq_t));
}

/* Generate the next term of recurrence A if required. */
static inline void maybe_generate_a(void) {
    if (a_out >= a_len) {
        seq_t next = (seq_t)194 * a_buf[a_len - 1] - a_buf[a_len - 2];
        ensure_cap(a_len + 1, &a_cap, &a_buf);
        a_buf[a_len++] = next;
    }
}

/* Generate the next term of recurrence B if required. */
static inline void maybe_generate_b(void) {
    if (b_out >= b_len) {
        seq_t next = (seq_t)9602 * b_buf[b_len - 1] - b_buf[b_len - 2];
        ensure_cap(b_len + 1, &b_cap, &b_buf);
        b_buf[b_len++] = next;
    }
}

/* Merge the two recurrences up to the required index n. */
static void ensure_merged_upto(idx_t n) {
    /* Initialize the first two terms of each sequence lazily. */
    if (a_len == 0) {
        ensure_cap(2, &a_cap, &a_buf);
        a_buf[0] = (seq_t)14;
        a_buf[1] = (seq_t)2702;
        a_len = 2;
    }
    if (b_len == 0) {
        ensure_cap(2, &b_cap, &b_buf);
        b_buf[0] = (seq_t)98;
        b_buf[1] = (seq_t)940898;
        b_len = 2;
    }
    if (merged_len == 0) {
        ensure_cap(16, &merged_cap, &merged);
        a_out = b_out = 0;
    }

    while (merged_len < (size_t)n) {
        maybe_generate_a();
        maybe_generate_b();

        seq_t a_val = a_buf[a_out];
        seq_t b_val = b_buf[b_out];

        if (a_val <= b_val) {
            ensure_cap(merged_len + 1, &merged_cap, &merged);
            merged[merged_len++] = a_val;
            ++a_out;
        } else {
            ensure_cap(merged_len + 1, &merged_cap, &merged);
            merged[merged_len++] = b_val;
            ++b_out;
        }
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* offset starts at 1 */
    ensure_merged_upto(n);
    return merged[n - 1];
}
