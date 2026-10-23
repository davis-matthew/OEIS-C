/*
 * A049910  a(n) = a(1) + a(2) + ... + a(n-1) - a(m) for n >= 4, where m = n -
 *          1 - 2^p and p is the unique integer such that 2^p < n - 1 <=
 *          2^(p+1), starting with a(1) = 1, a(2) = 2, and a(3) = 3.
 * https://oeis.org/A049910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=2, a(3)=3.
   For n ≥ 4:  let t = n‑1, p = ⌊log₂(t‑1)⌋,
   m = t – 2^p; then a(n) = S(t) – a(m), where S(k)=a(1)+…+a(k). */

#include <stdlib.h>     /* malloc, realloc, abort */

static seq_t *a_vals = NULL;   /* a(n) values, 1‑based */
static seq_t *a_sums = NULL;   /* cumulative sums S(n), 1‑based */
static idx_t  a_cap  = 0;      /* allocated capacity (elements) */
static idx_t  a_max  = 0;      /* largest n that has been computed */

/* Return ⌊log₂(v)⌋ for v>0. */
static idx_t floor_log2_uint64(idx_t v)
{
    idx_t r = 0;
    while (v > 1) {
        v >>= 1;
        ++r;
    }
    return r;
}

/* Grow the cached tables to hold at least 'need' elements (including index 0). */
static void ensure_capacity(idx_t need)
{
    if (need <= a_cap) return;
    idx_t new_cap = a_cap ? a_cap << 1 : 8;
    while (new_cap < need) new_cap <<= 1;
    a_vals = (seq_t *)realloc(a_vals, new_cap * sizeof(seq_t));
    a_sums = (seq_t *)realloc(a_sums, new_cap * sizeof(seq_t));
    if (!a_vals || !a_sums) abort();  /* allocation failed */
    for (idx_t i = a_cap; i < new_cap; ++i) {
        a_vals[i] = (seq_t)0;
        a_sums[i] = (seq_t)0;
    }
    a_cap = new_cap;
}

/* Generate all terms up to n (inclusive). */
static void compute_upto(idx_t n)
{
    if (n <= a_max) return;
    ensure_capacity(n + 1);          /* we index from 1, need n+1 slots */
    a_sums[0] = (seq_t)0;            /* zero‑index for sums */

    for (idx_t i = a_max + 1; i <= n; ++i) {
        if (i == 1) {
            a_vals[1] = (seq_t)1;
            a_sums[1] = (seq_t)1;
        } else if (i == 2) {
            a_vals[2] = (seq_t)2;
            a_sums[2] = (seq_t)3;
        } else if (i == 3) {
            a_vals[3] = (seq_t)3;
            a_sums[3] = (seq_t)6;
        } else {
            idx_t t   = i - 1;                           /* n-1 */
            idx_t p   = floor_log2_uint64(t - 1);          /* 2^p < t <= 2^(p+1) */
            idx_t hi_idx = ((idx_t)1) << p;                /* 2^p as idx_t */
            seq_t hi = (seq_t)hi_idx;                     /* cast to seq_t safely */
            idx_t m   = t - hi_idx;                      /* m = n-1-2^p */
            seq_t val = a_sums[i - 1] - a_vals[m];       /* a(n) */
            a_vals[i] = val;
            a_sums[i] = a_sums[i - 1] + val;
        }
    }
    a_max = n;
}

/* Public interface required by the framework. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0; /* undefined for n<1 */
    compute_upto(n);
    return a_vals[n];
}
