/*
 * A083592  Inverse binomial transform of A083590.
 * https://oeis.org/A083592
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -a(n-1) +10*a(n-2)+30*a(n-3)+35*a(n-4)+20*a(n-5) with
 *            initial values a(0)=1, a(1)=4, a(2)=16, a(3)=64, a(4)=256,
 *            a(5)=1025 (derived from the rational generating function of the
 *            sequence)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init[6] = {
    (seq_t)1,
    (seq_t)4,
    (seq_t)16,
    (seq_t)64,
    (seq_t)256,
    (seq_t)1025
};

static seq_t *seq = NULL;          /* cached sequence values */
static idx_t  seq_size  = 0;       /* number of computed values */
static idx_t  seq_cap   = 0;       /* capacity of the allocated array */

static void ensure_capacity(idx_t need) {
    if (seq_cap <= need) {
        idx_t new_cap = (seq_cap ? seq_cap * 2 : 6);
        if (new_cap < need + 1) new_cap = need + 1;
        seq = (seq_t *)realloc(seq, (size_t)new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
}

seq_t a(idx_t n) {
    /* initialise cache on first call */
    if (seq_size == 0) {
        ensure_capacity(6);
        for (idx_t i = 0; i < 6; ++i) seq[i] = init[i];
        seq_size = 6;
    }

    /* if already computed, return immediately */
    if (n < seq_size) return seq[n];

    /* compute missing terms up to n */
    ensure_capacity(n);
    for (idx_t i = seq_size; i <= n; ++i) {
        /* recurrence: a[i] = 10*a[i-2] + 30*a[i-3] + 35*a[i-4] + 20*a[i-5] - a[i-1] */
        seq[i] =
            seq[i - 2] * (seq_t)10 +
            seq[i - 3] * (seq_t)30 +
            seq[i - 4] * (seq_t)35 +
            seq[i - 5] * (seq_t)20 -
            seq[i - 1];
    }
    seq_size = n + 1;
    return seq[n];
}
