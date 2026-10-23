/*
 * A022434  a(n) = c(n-1) + c(n-3) where c is the sequence of numbers not in
 *          a.
 * https://oeis.org/A022434
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A022434
    a(n) = c(n-1) + c(n-3)
    where c is the increasing sequence of positive integers that are not in a.
    a(0)=1, a(1)=3, a(2)=6.  The function is memoised and works for calls
    in any order. */

#include <stdbool.h>
#include <stdlib.h>

/*= static storage =*/
static seq_t *a_seq  = NULL;      /* a sequence   */
static seq_t *c_seq  = NULL;      /* complement c */
static idx_t  a_len  = 0;         /* used size of a_seq */
static idx_t  c_len  = 0;         /* used size of c_seq */
static size_t a_cap  = 0;         /* allocated capacity of a_seq */
static size_t c_cap  = 0;         /* allocated capacity of c_seq */
static idx_t  next_candidate = 1; /* next integer to test for c */

/*= helpers =*/
/* binary search on the sorted a_seq array   */
static inline bool a_contains(seq_t val)
{
    idx_t low = 0;
    idx_t high = a_len;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        seq_t midval = a_seq[mid];
        if (midval == val) return true;
        if (midval < val)
            low = mid + 1;
        else
            high = mid;
    }
    return false;
}

/* grow a_seq to at least 'needed' elements */
static void ensure_a_cap(size_t needed)
{
    if (needed <= a_cap) return;
    size_t new_cap = a_cap ? a_cap : 8;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_arr = (seq_t *)realloc(a_seq, new_cap * sizeof(seq_t));
    a_seq = new_arr;
    a_cap = new_cap;
}

/* grow c_seq to at least 'needed' elements */
static void ensure_c_cap(size_t needed)
{
    if (needed <= c_cap) return;
    size_t new_cap = c_cap ? c_cap : 8;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_arr = (seq_t *)realloc(c_seq, new_cap * sizeof(seq_t));
    c_seq = new_arr;
    c_cap = new_cap;
}

/* initialise the sequence once */
static void init_sequence(void)
{
    if (a_seq == NULL) {
        a_cap = 8;
        a_seq = (seq_t *)malloc(a_cap * sizeof(seq_t));
    }
    if (c_seq == NULL) {
        c_cap = 8;
        c_seq = (seq_t *)malloc(c_cap * sizeof(seq_t));
    }

    /* first three terms of a */
    a_seq[0] = (seq_t)1;
    a_seq[1] = (seq_t)3;
    a_seq[2] = (seq_t)6;
    a_len = 3;

    /* first three terms of c (2,4,5) */
    next_candidate = 1;
    while (c_len < 3) {
        while (a_contains((seq_t)next_candidate))
            ++next_candidate;
        ensure_c_cap((size_t)c_len + 1);
        c_seq[c_len++] = (seq_t)next_candidate;
        ++next_candidate;
    }
}

/*= public function =*/
seq_t a(idx_t n)
{
    if (a_seq == NULL) init_sequence();

    if (n < a_len) return a_seq[n];

    for (idx_t i = a_len; i <= n; ++i) {
        /* guarantee c has terms up to index i-1 */
        while (c_len <= i - 1) {
            while (a_contains((seq_t)next_candidate))
                ++next_candidate;
            ensure_c_cap((size_t)c_len + 1);
            c_seq[c_len++] = (seq_t)next_candidate;
            ++next_candidate;
        }

        seq_t val = c_seq[i - 1] + c_seq[i - 3];
        ensure_a_cap((size_t)i + 1);
        a_seq[i] = val;
        ++a_len;
    }

    return a_seq[n];
}
