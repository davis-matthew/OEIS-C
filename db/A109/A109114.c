/*
 * A109114  a(n) = 5*a(n-1) - 3*a(n-2), a(0)=1, a(1)=6.
 * https://oeis.org/A109114
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 3*a(n-2), a(0)=1, a(1)=6
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq = NULL;          /* array of computed terms */
static size_t seq_cap = 0;         /* current allocated capacity */
static idx_t last_comp = (idx_t)-1;/* last index for which seq[last_comp] is defined */

/* Ensure the array has space for at least indices [0..n] and that the
   initial terms are set.  Returns after computing all missing terms up to n. */
static void ensure_terms(idx_t n)
{
    if (seq == NULL) {
        /* initial capacity at least 2 */
        seq_cap = (n < 2 ? 2 : n + 1);
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq) abort();
        seq[0] = (seq_t)1u;
        seq[1] = (seq_t)6u;
        last_comp = 1;
    }

    /* grow the array if needed */
    while (n >= seq_cap) {
        size_t new_cap = seq_cap * 2;
        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        seq = tmp;
        seq_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = last_comp + 1; i <= n; ++i) {
        seq[i] = seq[i-1] * (seq_t)5u - seq[i-2] * (seq_t)3u;
    }
    if (n > last_comp) last_comp = n;
}

seq_t a(idx_t n)
{
    ensure_terms(n);
    return seq[n];
}
