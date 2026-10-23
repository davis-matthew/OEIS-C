/*
 * A176747  Triangular numbers and numbers which cannot be represented as a
 *          sum of two earlier members of the sequence.
 * https://oeis.org/A176747
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A176747 – Triangular numbers and numbers which cannot be represented
 * as the sum of two earlier members of the sequence.
 *
 * Formula: a(n) = smallest integer m > a(n‑1) such that
 *   (m is triangular) OR (m cannot be expressed as a sum of two earlier a(i))
 *   for all i < j < n.
 *
 * The implementation maintains the sequence in a dynamic array and a
 * bit‑array (`can_sum`) that marks all values that are representable as a
 * sum of two earlier terms.  When a new sequence member is appended, all
 * sums involving the new value are marked.  Triangularity is tested by
 * checking whether 8*m+1 is a perfect square.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include "../utils/floor_sqrt.h"   /* floor_sqrt(seq_t) returns ⌊√x⌋ */

/* static state shared by all calls of a() */
static seq_t *seq = NULL;          /* array of sequence values        */
static size_t seq_len = 0, seq_cap = 0;
static unsigned char *can_sum = NULL; /* can_sum[i] == 1 iff i is a sum of two earlier terms */
static size_t can_len = 0;
static idx_t candidate = 1;        /* next integer to test            */

/* ensure can_sum has at least min_len bytes, all zero‑filled */
static void ensure_can_len(size_t min_len)
{
    if (min_len <= can_len) return;
    size_t new_len = can_len ? can_len << 1 : 16;
    while (new_len <= min_len) new_len <<= 1;
    unsigned char *tmp = (unsigned char *)realloc(can_sum, new_len);
    if (!tmp) abort();
    if (new_len > can_len)
        memset(tmp + can_len, 0, new_len - can_len);
    can_sum = tmp;
    can_len = new_len;
}

/* append a new value to the sequence and update the sums table */
static void add_term(seq_t val)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap << 1 : 16;
        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        seq = tmp;
        seq_cap = new_cap;
    }
    seq[seq_len++] = val;

    /* mark all sums involving the new value with any previous term,
       including the new value itself (irrelevant but harmless) */
    for (size_t i = 0; i < seq_len; ++i) {
        seq_t sum = seq[i] + val;
        /* convert sum to an index for can_sum; the sequence values
           for n≤10000 are small enough to fit in idx_t/size_t */
        idx_t sum_idx = (idx_t)sum;
        if ((size_t)sum_idx >= can_len) ensure_can_len((size_t)sum_idx + 1);
        can_sum[(size_t)sum_idx] = 1;
    }
}

/* test if x is triangular: 8*x+1 is a perfect square */
static bool is_triangular(seq_t x)
{
    seq_t v = 8 * x + 1;
    seq_t r = floor_sqrt(v);
    return r * r == v;
}

/* public API */
seq_t a(idx_t n)
{
    /* initialize on first call */
    if (!seq) {
        seq_cap = 16;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq) abort();
        seq_len = 0;
        can_len = 0;
        can_sum = NULL;
        add_term((seq_t)0);          /* a(0) = 0 */
        candidate = 1;
    }

    /* extend the sequence until we have at least n+1 terms */
    while (seq_len <= (size_t)n) {
        if ((size_t)candidate >= can_len) ensure_can_len((size_t)candidate + 1);
        bool add = false;
        if (is_triangular((seq_t)candidate))
            add = true;
        else if (!can_sum[(size_t)candidate])
            add = true;
        if (add)
            add_term((seq_t)candidate);
        ++candidate;
    }

    return seq[(size_t)n];
}
