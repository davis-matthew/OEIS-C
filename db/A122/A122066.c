/*
 * A122066  a(0)=1. a(n) = the number of earlier terms equal to GCD(a(k),n),
 *          where a(k) is the largest term among terms a(0) through a(n-1).
 * https://oeis.org/A122066
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 103, u16 n <= 103, u32 n <= 103, u64 n <= 103,
 *            u128 n <= 103, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Recurrence: a(0) = 1; for n > 0,
 *    let m = max { a(k) | 0 ≤ k < n };
 *    a(n) = #{ k | 0 ≤ k < n , a(k) = gcd(m, n) }.
 * This implementation memoizes computed terms and counts of each value.
 */
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* cached sequence */
static seq_t *seq      = NULL;
static size_t seq_len  = 0;  /* number of values stored */
static size_t seq_cap  = 0;  /* allocated capacity */

/* count of occurrences of each value: cnt[v] = #{ k | a(k)=v } */
static seq_t *cnt      = NULL;
static size_t cnt_cap  = 0;  /* allocated capacity for cnt */

/* current maximum among the sequence values */
static seq_t max_val = 0;

/* ensure seq has capacity for at least 'need' elements */
static void ensure_seq_cap(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap : 8;
    while (new_cap < need) new_cap <<= 1;
    seq_t *ptr = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!ptr) exit(1);
    seq = ptr;
    seq_cap = new_cap;
}

/* ensure cnt can index the given seq_t value */
static void ensure_cnt_cap(seq_t value)
{
    size_t val = (size_t)value;
    if (val < cnt_cap) return;
    size_t new_cap = cnt_cap ? cnt_cap : 8;
    while (new_cap <= val) new_cap <<= 1;
    seq_t *ptr = (seq_t *)realloc(cnt, new_cap * sizeof(seq_t));
    if (!ptr) exit(1);
    /* init newly allocated slots to zero */
    memset(ptr + cnt_cap, 0, (new_cap - cnt_cap) * sizeof(seq_t));
    cnt = ptr;
    cnt_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* one‑time initialization */
    if (seq_len == 0) {
        ensure_seq_cap(1);
        seq[0]          = 1;
        ensure_cnt_cap(1);
        cnt[1]          = 1;
        max_val         = 1;
        seq_len         = 1;
    }

    /* return already known term */
    if ((size_t)n < seq_len) return seq[n];

    /* compute missing terms up to n */
    for (size_t i = seq_len; i <= (size_t)n; ++i) {
        seq_t gi = gcd(max_val, (seq_t)i);
        size_t gi_idx = (size_t)gi;
        ensure_cnt_cap((seq_t)gi_idx);
        seq_t count = cnt[gi_idx];

        ensure_seq_cap(i + 1);
        seq[i] = count;

        /* increment count for this value */
        size_t count_idx = (size_t)count;
        ensure_cnt_cap(count);
        cnt[count_idx]++;

        if (count > max_val) max_val = count;
    }

    seq_len = (size_t)n + 1;
    return seq[n];
}
