/*
 * A078023  Expansion of (1-x)/(1-2*x^2-2*x^3).
 * https://oeis.org/A078023
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=1, a(1)=-1, a(2)=2; a(n)=2*a(n-2)+2*a(n-3) for n≥3
 * Range:     i8 n <= 11, i16 n <= 21, i32 n <= 40, i64 n <= 79, i128 n <= 157,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_seq = NULL;   /* memoized terms */
static idx_t a_len = 0;       /* number of terms computed */
static idx_t a_cap = 0;       /* allocated capacity */

static void ensure_capacity(idx_t needed)
{
    if (needed <= a_cap) return;
    idx_t new_cap = a_cap < 8 ? 8 : a_cap * 2;
    while (new_cap < needed) new_cap *= 2;
    a_seq = (seq_t *)realloc(a_seq, new_cap * sizeof(seq_t));
    a_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-1;
    if (n == 2) return (seq_t)2;

    /* initialise memoisation storage on first call */
    if (a_len == 0) {
        ensure_capacity(3);
        a_seq[0] = (seq_t)1;
        a_seq[1] = (seq_t)-1;
        a_seq[2] = (seq_t)2;
        a_len = 3;
    }

    /* compute missing terms up to n */
    if (n >= a_len) {
        ensure_capacity(n + 1);
        for (idx_t i = a_len; i <= n; ++i) {
            a_seq[i] = (seq_t)2 * a_seq[i - 2] + (seq_t)2 * a_seq[i - 3];
        }
        a_len = n + 1;
    }

    return a_seq[n];
}
