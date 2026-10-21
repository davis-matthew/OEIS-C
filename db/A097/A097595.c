/*
 * A097595  a(0) = 2, a(1) = 5, a(n) = a(n-2)^2 + a(n-1)*a(n-2).
 * https://oeis.org/A097595
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=2, a(1)=5, a(n)=a(n-2)^2 + a(n-1)*a(n-2)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq = NULL;
static size_t seq_cap = 0;
static size_t seq_len = 0;

seq_t a(idx_t n)
{
    /* Ensure we have storage for indices 0..n */
    size_t needed = (size_t)(n + 1);
    if (seq == NULL) {
        seq_cap = needed > 2 ? needed : 2;   /* start with at least 2 */
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (seq == NULL) exit(1);
    } else if (seq_cap < needed) {
        while (seq_cap < needed) seq_cap = seq_cap ? seq_cap * 2 : 2;
        seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
        if (seq == NULL) exit(1);
    }

    /* Initialize first two terms if not already done */
    if (seq_len == 0) {
        seq[0] = (seq_t)2;
        seq_len = 1;
    }
    if (seq_len == 1 && seq_cap >= 2) {
        seq[1] = (seq_t)5;
        seq_len = 2;
    }

    /* Compute missing terms up to n */
    for (idx_t i = (idx_t)seq_len; i <= n; ++i) {
        seq[i] = seq[i-2] * seq[i-2] + seq[i-1] * seq[i-2];
    }
    seq_len = (size_t)(n + 1);
    return seq[n];
}
