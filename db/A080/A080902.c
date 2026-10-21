/*
 * A080902  a(1)=1, a(n)=a(n-1)+2 if (n and n+3 are in the sequence),
 *          a(n)=a(n-1)+3 otherwise.
 * https://oeis.org/A080902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; For n>1, a(n)=a(n-1)+2 if both n and n+3 appear as
 *            sequence terms, otherwise a(n)=a(n-1)+3
 * Range:     u8 n <= 94, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* static storage for the sequence */
static seq_t *seq = NULL;           /* array of sequence values          */
static size_t seq_len = 0;          /* number of terms currently cached  */
static size_t seq_cap = 0;          /* allocated capacity of seq array   */

/* binary search in the cached sequence; returns true if v is a term  */
static bool is_present(seq_t v)
{
    size_t lo = 0, hi = seq_len;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (seq[mid] == v)
            return true;
        if (seq[mid] < v)
            lo = mid + 1;
        else
            hi = mid;
    }
    return false;
}

/* ensure that seq contains at least n terms (n>=1) */
static void ensure_terms(idx_t n)
{
    if (seq_len >= (size_t)n)
        return;

    /* grow the sequence array if needed */
    if (n > seq_cap) {
        size_t new_cap = n * 2;            /* at least double */
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }

    /* initialise the first term if empty */
    if (seq_len == 0) {
        seq[0] = (seq_t)1;
        seq_len = 1;
    }

    /* generate missing terms */
    for (size_t i = seq_len; i < (size_t)n; ++i) {
        /* current index (1‑based) */
        idx_t curr_idx = (idx_t)(i + 1);
        seq_t curr_val  = (seq_t)curr_idx;
        seq_t plus3_val = curr_val + (seq_t)3;

        bool inc2 = is_present(curr_val) && is_present(plus3_val);
        seq_t inc = inc2 ? (seq_t)2 : (seq_t)3;
        seq[i] = seq[i - 1] + inc;
        seq_len = i + 1;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)  /* sequence is defined only for n>=1 */
        return 0;
    ensure_terms(n);
    return seq[n - 1];
}
