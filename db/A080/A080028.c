/*
 * A080028  Let s(1) = 1; let s(2m+1) = {s(2m),m+1,s(2m)}, s(2m) =
 *          {s(2m-1),s(2m-1)}; sequence gives limit of s(k) for large k.
 * https://oeis.org/A080028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the limit of the recursively defined sequence * s(1) =
 *            1, * s(2m) = s(2m-1)∥s(2m-1) (duplication) * s(2m+1) =
 *            s(2m)∥(m+1)∥s(2m) (insertion) * a(n) is the eventual value at
 *            position n in s(k) as k→∞. * The implementation computes a(n) by
 *            first determining the smallest * k with |s(k)| ≥ n,...
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Ensure arrays for lengths are available up to a needed index */
static seq_t *S_len = NULL;        /* S_len[k] = |s(k)| (1‑based) */
static idx_t  S_len_cnt = 0;       /* number of computed lengths (max k) */
static size_t S_len_cap = 0;       /* allocated capacity for S_len */

/* Extend the length table to cover index n */
static void ensure_len(idx_t n)
{
    if (S_len_cnt == 0) {
        /* initialise */
        S_len_cap = 4;
        S_len = (seq_t *)malloc(sizeof(seq_t) * (S_len_cap + 1));
        if (!S_len) exit(1);
        S_len_cnt = 1;
        S_len[1] = (seq_t)1;
    }

    while (S_len[S_len_cnt] < (seq_t)n) {
        /* grow capacity if needed */
        if (S_len_cnt >= S_len_cap) {
            size_t new_cap = S_len_cap * 2;
            seq_t *tmp = (seq_t *)realloc(S_len, sizeof(seq_t) * (new_cap + 1));
            if (!tmp) exit(1);
            S_len = tmp;
            S_len_cap = new_cap;
        }
        idx_t k = S_len_cnt + 1;
        if ((k & 1) == 0)            /* even   */
            S_len[k] = S_len[k-1] * ((seq_t)2);
        else                          /* odd    */
            S_len[k] = S_len[k-1] * ((seq_t)2) + (seq_t)1;
        S_len_cnt = k;
    }
}

/* Recursive (iterative) evaluation of the value at position 'pos'
 * inside s(k), where k is such that |s(k)| ≥ pos. */
static seq_t value_at(idx_t k, idx_t pos)
{
    seq_t posS = (seq_t)pos;   /* use seq_t for arithmetic with lengths */
    while (k > 1) {
        if ((k & 1) == 0) {        /* even: duplication */
            seq_t t = S_len[k-1];
            if (posS > t)
                posS -= t;
            k -= 1;
        } else {                   /* odd: insertion */
            seq_t t   = S_len[k-1];
            seq_t mid = t + (seq_t)1;               /* position of inserted value */
            if (posS == mid)
                return (seq_t)((k-1)/2 + (seq_t)1); /* (m+1) */
            else if (posS < mid) {
                k -= 1;
            } else {
                posS -= mid;
                k -= 1;
            }
        }
    }
    return (seq_t)1;       /* base s(1) = 1 */
}

/* Public function required by the exercise */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* not used but safe */
    ensure_len(n);
    /* find smallest k such that |s(k)| ≥ n */
    idx_t k = 1;
    while (S_len[k] < (seq_t)n) k++;
    return value_at(k, n);
}
