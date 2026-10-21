/*
 * A052555  Expansion of e.g.f. 1/(1-2*x-x^2).
 * https://oeis.org/A052555
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=2, a(n) = 2*n*a(n-1) + n*(n-1)*a(n-2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>             /* for malloc / realloc */

static seq_t *seq = NULL;      /* memoisation buffer */
static size_t seq_len = 0;     /* number of terms currently stored */
static size_t seq_cap = 0;     /* capacity of the buffer */

/* ensure the buffer can hold at least `needed` elements */
static void ensure_cap(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 4;
    while (new_cap < needed) new_cap *= 2;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* initialise first two terms if needed */
    if (seq_len == 0) {
        ensure_cap(2);
        seq[0] = (seq_t)1;
        seq[1] = (seq_t)2;
        seq_len = 2;
    }

    /* extend memoisation up to n if required */
    if ((size_t)n >= seq_len) {
        ensure_cap((size_t)n + 1);
        for (idx_t i = seq_len; i <= n; ++i) {
            /* compute 2*i*a(i-1) + i*(i-1)*a(i-2) */
            seq_t term1 = (seq_t)i * (seq_t)2 * seq[i - 1];
            seq_t term2 = (seq_t)i * (seq_t)(i - 1) * seq[i - 2];
            seq[i] = term1 + term2;
        }
        seq_len = (size_t)n + 1;
    }

    return seq[n];
}
