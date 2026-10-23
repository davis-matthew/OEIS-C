/*
 * A154151  Indices k such that 25 plus the k-th triangular number is a
 *          perfect square.
 * https://oeis.org/A154151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less, easy
 * Formula:   a(n) = k such that 25 + k(k+1)/2 is a perfect square.
 *            Computation is performed using the recurrence
 *            a(n)=a(n-1)+6*a(n-2)-6*a(n-3)-a(n-4)+a(n-5). Initial terms:
 *            a(1)=0, a(2)=18, a(3)=21, a(4)=111, a(5)=128
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 49, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq      = NULL;   /* cached values of the sequence   */
static size_t seq_len  = 0;      /* number of stored values         */
static size_t seq_cap  = 0;      /* current allocation size         */

static const seq_t init[5] = {
    (seq_t)0,   /* a(1) */
    (seq_t)18,  /* a(2) */
    (seq_t)21,  /* a(3) */
    (seq_t)111, /* a(4) */
    (seq_t)128  /* a(5) */
};

static void ensure_capacity(size_t min_cap) {
    if (seq_cap >= min_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 8;
    while (new_cap < min_cap) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp && new_cap > 0) abort();      /* out of memory */
    seq = tmp;
    seq_cap = new_cap;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;        /* undefined, return 0 for safety */

    /* Initialise cache with the first five terms on first call */
    if (seq_len == 0) {
        ensure_capacity(8);
        for (size_t i = 0; i < 5; ++i) seq[i] = init[i];
        seq_len = 5;
    }

    /* Generate terms until we reach the requested index */
    while (seq_len < (size_t)n) {
        /* compute a(seq_len+1) using the recurrence */
        seq_t term = seq[seq_len - 1];                   /* a(n-1)   */
        term += seq[seq_len - 2] * (seq_t)6;             /* +6*a(n-2) */
        term -= seq[seq_len - 3] * (seq_t)6;             /* -6*a(n-3) */
        term -= seq[seq_len - 4];                       /* -a(n-4)   */
        term += seq[seq_len - 5];                       /* +a(n-5)   */

        /* ensure capacity and store the new term */
        ensure_capacity(seq_len + 1);
        seq[seq_len] = term;
        ++seq_len;
    }

    return seq[n - 1];
}
