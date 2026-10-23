/*
 * A132832  Largest integer terms forming a self-convolution square-root of a
 *          sequence A132831 such that: A132831(n) <= 2*A132831(n-1) for n>0
 *          with A132831(0)=1.
 * https://oeis.org/A132832
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the largest integer such that the self‑convolution
 *            c(n)=∑_{i=0}^n a(i)·a(n-i) satisfies c(n) ≤ 2·c(n-1).
 *            Recurrence: a(0)=1, a(n)=⌊(2·c(n-1)-∑_{i=1}^{n-1}
 *            a(i)·a(n-i))/2⌋, c(n)=2·a(n)+∑_{i=1}^{n-1} a(i)·a(n-i)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *a_seq   = NULL;   /* store a(i) */
    static seq_t *c_seq   = NULL;   /* store c(i) */
    static size_t capacity = 0;     /* allocated size */
    static idx_t  computed  = 0;    /* next index to compute */

    /* initial allocation and first term */
    if (!computed) {
        capacity = 8;
        a_seq = (seq_t*)malloc(capacity * sizeof(seq_t));
        c_seq = (seq_t*)malloc(capacity * sizeof(seq_t));
        a_seq[0] = (seq_t)1;
        c_seq[0] = a_seq[0] * a_seq[0];   /* c(0)=1 */
        computed = 1;                     /* next term to compute is 1 */
    }

    /* grow arrays if needed */
    if (n >= capacity) {
        size_t newcap = capacity;
        while (n >= newcap) newcap <<= 1;     /* double until large enough */
        a_seq = (seq_t*)realloc(a_seq, newcap * sizeof(seq_t));
        c_seq = (seq_t*)realloc(c_seq, newcap * sizeof(seq_t));
        capacity = newcap;
    }

    /* compute missing terms up to n */
    for (idx_t k = computed; k <= n; ++k) {
        seq_t S = 0;                       /* Σ_{i=1}^{k-1} a(i)*a(k-i) */
        for (idx_t i = 1; i < k; ++i)
            S += a_seq[i] * a_seq[k - i];

        seq_t c_prev = c_seq[k - 1];
        seq_t ak = (c_prev * 2 - S) / 2;    /* largest integer satisfying the bound */
        a_seq[k] = ak;
        c_seq[k] = ak * 2 + S;             /* update convolution value */
    }
    computed = n + 1;                      /* all terms up to n are now ready */

    return a_seq[n];
}
