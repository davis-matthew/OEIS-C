/*
 * A170367  Number of reduced words of length n in Coxeter group on 22
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^43 = I.
 * https://oeis.org/A170367
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A170367:
   a(0) = 1
   For 1 ≤ n ≤ 42:  a(n) = 20 * (a(0)+…+a(n‑1)) + 2
   a(43) = 1 + 20 * (a(1)+…+a(42)) – 210 * a(0)
   For n ≥ 44:   a(n) = 20 * (a(n‑42)+…+a(n‑1)) – 210 * a(n‑43)   */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *seq = NULL;
    static size_t allocated = 0;
    static idx_t cached_max = (idx_t)-1;   /* sentinel: nothing computed yet */
    static seq_t sum_all = 0;              /* sum of seq[0..cached_max] */

    /* allocate or grow cache to hold indices up to n */
    if (!seq) {
        allocated = n + 16;
        if (allocated < 64) allocated = 64;
        seq = (seq_t *)malloc(allocated * sizeof(seq_t));
        if (!seq) return (seq_t)0;
    } else if ((size_t)(n + 1) > allocated) {
        size_t new_cap = allocated * 2;
        if (new_cap < (size_t)(n + 1)) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        seq = tmp;
        allocated = new_cap;
    }

    /* if nothing computed yet, start at 0 else continue after cached_max */
    idx_t start = (cached_max == (idx_t)-1) ? 0 : cached_max + 1;

    for (idx_t i = start; i <= n; ++i) {
        if (i == 0) {
            seq[0] = (seq_t)1;
            sum_all = seq[0];
        }
        else if (i <= 42) {
            seq[i] = (seq_t)20 * sum_all + (seq_t)2;
            sum_all += seq[i];
        }
        else if (i == 43) {
            /* sum of a(1) … a(42) = sum_all - a(0) */
            seq_t sum_first_42 = sum_all - seq[0];
            seq[43] = (seq_t)1 + (seq_t)20 * sum_first_42 - (seq_t)210 * seq[0];
            sum_all += seq[43];
        }
        else { /* i >= 44 */
            /* compute sum of last 42 terms: a(i-42) … a(i-1) */
            seq_t window_sum = 0;
            idx_t j_start = i - 42;  /* i ≥ 44 ⇒ j_start ≥ 2 */
            for (idx_t j = j_start; j < i; ++j)
                window_sum += seq[j];
            seq[i] = (seq_t)20 * window_sum - (seq_t)210 * seq[i - 43];
            sum_all += seq[i];
        }
    }

    cached_max = n;
    return seq[n];
}
