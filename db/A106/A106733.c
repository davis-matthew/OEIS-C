/*
 * A106733  a(n) = a(a(a(a(n - a(n-1))))) + a(n - a(n-2)) with a(1) = a(2) =
 *          1.
 * https://oeis.org/A106733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(a(a(a(n - a(n-1))))) + a(n - a(n-2)), initial values:
 *            a(1)=1, a(2)=1
 * Range:     u8 n <= 8996, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, abort */

static void
ensure_capacity(idx_t min_cap, seq_t **arr, size_t *cap)
{
    if (*cap >= (size_t)min_cap) return;
    size_t new_cap = (*cap == 0) ? 1 : *cap;
    while (new_cap < (size_t)min_cap) new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    if (!tmp) abort();                 /* out of memory */
    *arr = tmp;
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;       /* undefined for n<1 */

    static seq_t *seq = NULL;          /* 0-based: seq[0] = a(1) */
    static size_t cap   = 0;           /* allocated size      */
    static size_t sz    = 0;           /* number of values stored */

    /* Return cached value if already computed */
    if (n <= sz) return seq[n - 1];

    /* Ensure capacity for n elements */
    ensure_capacity(n, &seq, &cap);

    /* Initialize the first two values if necessary */
    if (sz < 2) {
        ensure_capacity(2, &seq, &cap);
        seq[0] = (seq_t)1; /* a(1) */
        seq[1] = (seq_t)1; /* a(2) */
        sz = 2;
    }

    /* Compute from (sz+1) up to n */
    for (idx_t i = sz + 1; i <= n; ++i) {
        seq_t a_nm1 = seq[i - 2]; /* a(i-1) */
        seq_t a_nm2 = seq[i - 3]; /* a(i-2) */

        /* Compute nested a(a(a(a(i - a(i-1))))) */
        idx_t t1 = i - (idx_t)a_nm1;                  /* i - a(i-1) */
        seq_t r1 = seq[t1 - 1];                      /* a(t1) */

        idx_t idx2 = (idx_t)r1 - 1;
        seq_t r2 = seq[idx2];                        /* a(r1) */

        idx_t idx3 = (idx_t)r2 - 1;
        seq_t r3 = seq[idx3];                        /* a(r2) */

        idx_t idx4 = (idx_t)r3 - 1;
        seq_t nested = seq[idx4];                    /* a(r3) */

        /* Second part: a(i - a(i-2)) */
        idx_t t2 = i - (idx_t)a_nm2;                  /* i - a(i-2) */
        seq_t part2 = seq[t2 - 1];                   /* a(t2) */

        seq[i - 1] = nested + part2;
    }

    sz = n;
    return seq[n - 1];
}
