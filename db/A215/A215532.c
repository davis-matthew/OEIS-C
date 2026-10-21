/*
 * A215532  The limit of the string "0, 1" under the operation 'append first k
 *          terms, increment k' with k=2 initially.
 * https://oeis.org/A215532
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8943, u16 n <= 8943, u32 n <= 8943, u64 n <= 8943,
 *            u128 n <= 8943, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A215532: a(n) is the n‑th bit of the infinite binary word constructed
   by starting with (0,1), then repeatedly appending the first k terms of the
   current word and incrementing k (initially k = 2). */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *word_buf = NULL;   /* buffer holding the generated bits */
static size_t   buf_cap  = 0;    /* current capacity of word_buf   */
static size_t   word_len = 0;    /* number of generated bits      */
static idx_t    cur_k    = 2;    /* current k (number of terms to append) */
static bool     initialized = false;

seq_t a(idx_t n)
{
    /* Lazy initialization of the sequence builder */
    if (!initialized) {
        initialized = true;
        buf_cap = 16;
        word_buf = (seq_t *)malloc(buf_cap * sizeof(*word_buf));
        /* initial word: 0, 1 */
        word_buf[0] = (seq_t)0;
        word_buf[1] = (seq_t)1;
        word_len   = 2;
        cur_k      = 2;
    }

    /* Grow the word until we have at least n+1 elements */
    while (word_len <= (size_t)n) {
        size_t new_len = word_len + (size_t)cur_k;          /* new length after append */
        /* Ensure capacity */
        if (new_len > buf_cap) {
            size_t new_cap = buf_cap ? buf_cap * 2 : 16;
            while (new_cap < new_len) new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(word_buf, new_cap * sizeof(*word_buf));
            word_buf = tmp;
            buf_cap  = new_cap;
        }
        /* Append the first cur_k terms of the current word */
        for (size_t i = 0; i < (size_t)cur_k; ++i) {
            word_buf[word_len + i] = word_buf[i];
        }
        word_len = new_len;
        ++cur_k;   /* increment k for the next expansion */
    }

    return word_buf[(size_t)n];
}
