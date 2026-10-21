/*
 * A092334  For S a finite string of numbers, let M(S) denote the maximal
 *          number among them. Let a(1)=1. For n>1, a(n) is the greatest k
 *          such that the string a(1)a(2)...a(n-1) can be written in the form
 *          [x][y_1][y_2]...[y_k] where each y_i is of positive (but not
 *          necessarily equal) length and M(y_i)=M(y_j) f...
 * https://oeis.org/A092334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1; for n>1, a(n)=max_{i=1..n-1} (#occurrences of the maximum value in a[i..n-1]) */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;          /* a[1..len] */
    static seq_t *max_val = NULL;     /* maximum in suffix starting at i */
    static idx_t *cnt = NULL;         /* occurrences of that maximum in suffix */

    static size_t cap = 0;            /* current capacity of arrays */
    static idx_t len = 0;             /* number of terms already computed */

    if (n == 0) return (seq_t)0;      /* undefined, return 0 */

    /* Allocate / grow arrays so that indices up to n are valid */
    if (cap <= (size_t)n + 1) {
        size_t new_cap = cap ? cap : 16;
        while (new_cap <= (size_t)n + 1) new_cap <<= 1;
        seq     = (seq_t*)realloc(seq,     new_cap * sizeof(seq_t));
        max_val = (seq_t*)realloc(max_val, new_cap * sizeof(seq_t));
        cnt     = (idx_t*)realloc(cnt,     new_cap * sizeof(idx_t));
        cap = new_cap;
    }

    /* Initialize base case a(1)=1 */
    if (len == 0) {
        seq[1] = (seq_t)1;
        max_val[1] = seq[1];
        cnt[1] = 1;
        len = 1;
    }

    /* Compute terms up to n */
    for (idx_t i = len + 1; i <= n; ++i) {

        /* Determine a(i) from the current suffix counts */
        seq_t val = (seq_t)1;                 /* default for i==1 (never reached here) */
        if (i > 1) {
            idx_t best = 0;
            for (idx_t j = 1; j < i; ++j) {
                if (cnt[j] > best) best = cnt[j];
            }
            val = (seq_t)best;
        }
        seq[i] = val;

        /* Update statistics for all earlier suffixes that now include a(i) */
        idx_t j;
        for (j = i - 1; j > 0; --j) {
            if (val > max_val[j]) {
                max_val[j] = val;
                cnt[j] = 1;
            } else if (val == max_val[j]) {
                ++cnt[j];
            }
        }

        /* Statistics for the suffix starting at position i itself */
        max_val[i] = val;
        cnt[i] = 1;

        len = i;
    }

    return seq[n];
}
