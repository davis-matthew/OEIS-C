/*
 * A135123  Numbers such that the digital sum base 2 and the digital sum base
 *          3 and the digital sum base 6 all are equal.
 * https://oeis.org/A135123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 273, u32 n <= 1200, u64 n <= 1200,
 *            u128 n <= 1200, big any n
 * Helpers:   digital_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A135123: numbers n such that ds_2(n) = ds_3(n) = ds_6(n). Search implementation. */

#include <stdlib.h>
#include "../utils/digital_sum.h"

/* Static storage for computed terms */
static seq_t *seq  = NULL;     /* array of found terms */
static size_t seq_cap = 0;     /* current capacity of seq */
static size_t seq_cnt = 0;     /* number of terms stored */
static seq_t  last_cand = 0;   /* last candidate checked */

/* Append a new term, resizing the array if necessary */
static void add_term(seq_t val)
{
    if (seq_cnt >= seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 16;
        seq_t *tmp = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failed */
        seq = tmp;
        seq_cap = new_cap;
    }
    seq[seq_cnt++] = val;
}

/* Return the nth term (a(1) = 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* guard against undefined index */

    /* Keep generating terms until we have at least n terms. */
    while (seq_cnt < (size_t)n) {
        last_cand++;                     /* next candidate */
        seq_t cand = last_cand;

        int s2 = digital_sum(cand, 2);
        int s3 = digital_sum(cand, 3);
        int s6 = digital_sum(cand, 6);

        if (s2 == s3 && s3 == s6)
            add_term(cand);
    }

    return seq[(size_t)n - 1];
}
