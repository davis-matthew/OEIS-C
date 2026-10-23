/*
 * A025918  Expansion of 1/((1-x^8)*(1-x^9)*(1-x^10)).
 * https://oeis.org/A025918
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A025918: Number of partitions of n into parts 8, 9, and 10.
   Recurrence (used in code):
     a(n) = a(n-8) + a(n-9) + a(n-10)
            - a(n-17) - a(n-18) - a(n-19)
            + a(n-27)
   Initial values: a(0)=1, a(1)=a(2)=…=a(7)=0. */
#include <stdlib.h>

static seq_t *seq = NULL;      /* cache of computed values */
static size_t seq_len = 0;     /* number of initialized elements */
static size_t seq_cap = 0;     /* allocated capacity */

/* Ensure the cache can hold indices 0..n. */
static void ensure_capacity(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (need <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : need;
    if (new_cap < need)
        new_cap = need;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* In a real program you might abort or handle the error. */
        return;
    }
    seq = tmp;
    seq_cap = new_cap;
}

/* Pre‑compute a[0..27] by the standard DP for the parts 8, 9, 10. */
static void init_small(void)
{
    const int parts[3] = {8, 9, 10};
    const size_t maxinit = 27;
    seq_len = maxinit + 1;
    ensure_capacity(maxinit);
    for (size_t i = 0; i < seq_len; ++i)
        seq[i] = 0;
    seq[0] = (seq_t)1;
    for (int p = 0; p < 3; ++p) {
        int part = parts[p];
        for (size_t i = (size_t)part; i < seq_len; ++i)
            seq[i] = seq[i] + seq[i - part];
    }
}

/* Sequence function required by the judge. */
seq_t a(idx_t n)
{
    /* Lazy initialization of the cache. */
    if (seq_len == 0)
        init_small();

    /* Extend the cache up to n if needed. */
    if ((idx_t)seq_len <= n) {
        ensure_capacity(n);
        for (size_t i = seq_len; i <= (size_t)n; ++i) {
            seq_t val = 0;
            if (i >= 8) val += seq[i - 8];
            if (i >= 9) val += seq[i - 9];
            if (i >= 10) val += seq[i - 10];
            if (i >= 17) val -= seq[i - 17];
            if (i >= 18) val -= seq[i - 18];
            if (i >= 19) val -= seq[i - 19];
            if (i >= 27) val += seq[i - 27];
            seq[i] = val;
        }
        seq_len = (size_t)n + 1;
    }

    return seq[(size_t)n];
}
