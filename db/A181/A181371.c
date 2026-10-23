/*
 * A181371  Triangle read by rows: T(n,k) is the number of ternary words
 *          (i.e., finite sequences of 0's, 1's and 2's) of length n having k
 *          occurrences of 01's (0 <= k <= floor(n/2)).
 * https://oeis.org/A181371
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = Σ_{k=0}^{⌊n/2⌋} T(n,k) , where T(n,k) counts ternary
 *            words of length n containing exactly k subwords “01”. The
 *            triangle is read row‑by‑row and flattened
 * Range:     u8 n <= 11, u16 n <= 36, u32 n <= 132, u64 n <= 465,
 *            u128 n <= 1770, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


/* Static state for memoised generation */
static seq_t *flat = NULL;          /* flattened sequence a(n)          */
static idx_t flat_cap = 0;
static idx_t flat_size = 0;         /* number of elements stored       */

static seq_t *dp0 = NULL;           /* words ending with 0             */
static seq_t *dp1 = NULL;           /* words ending with 1             */
static idx_t cur_len = 0;           /* length of last processed row n   */

/* Allocate or reallocate a seq_t array */
static seq_t *alloc_seq(size_t n)
{
    return (seq_t*)calloc(n, sizeof(seq_t));
}
static seq_t *grow_seq(void *p, size_t new_cap)
{
    return (seq_t*)realloc(p, new_cap * sizeof(seq_t));
}

/* Append value to the flattened sequence */
static void append_flat(seq_t value)
{
    if (flat_cap <= flat_size) {
        size_t new_cap = flat_cap ? flat_cap * 2 : 16;
        seq_t *tmp = grow_seq(flat, new_cap);
        if (!tmp) abort();
        flat = tmp;
        flat_cap = new_cap;
    }
    flat[flat_size++] = value;
}

/* Generate the next triangle row */
static void generate_next_row(void)
{
    /* Length of the new row */
    idx_t n = cur_len + 1;
    idx_t new_kmax = n / 2;           /* floor(n/2)          */
    idx_t old_kmax = cur_len / 2;     /* floor((n‑1)/2)      */

    /* Allocate new DP arrays */
    seq_t *newdp0 = alloc_seq(new_kmax + 1);
    seq_t *newdp1 = alloc_seq(new_kmax + 1);
    if (!newdp0 || !newdp1) abort();

    /* Transition: newdp0[k] = 2*dp0[k] + dp1[k] */
    /*            newdp1[k] = dp1[k] + dp0[k] + (k>0?dp0[k‑1]:0) */
    for (idx_t k = 0; k <= new_kmax; ++k) {
        seq_t a0 = (k <= old_kmax) ? dp0[k] : 0;
        seq_t a1 = (k <= old_kmax) ? dp1[k] : 0;
        seq_t a0prev = (k > 0 && (k - 1) <= old_kmax) ? dp0[k - 1] : 0;

        newdp0[k] = a0 * 2 + a1;
        newdp1[k] = a1 + a0 + a0prev;
    }

    /* Append flattened values for the new row:
       T(n,k) = 2*newdp0[k] + newdp1[k]   */
    for (idx_t k = 0; k <= new_kmax; ++k) {
        seq_t val = newdp0[k] * 2 + newdp1[k];
        append_flat(val);
    }

    /* Free old DP arrays and promote new ones */
    free(dp0);
    free(dp1);
    dp0 = newdp0;
    dp1 = newdp1;
    cur_len = n;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    /* Initialise on first call: a(0) = 1, row n=1 states. */
    if (!flat) {
        flat_cap = 16;
        flat = (seq_t*)calloc(flat_cap, sizeof(seq_t));
        if (!flat) abort();
        flat[0] = 1;              /* a(0) */
        flat_size = 1;

        /* DP for row n=1: words "0", "1", "2" */
        dp0 = (seq_t*)calloc(1, sizeof(seq_t));
        dp1 = (seq_t*)calloc(1, sizeof(seq_t));
        if (!dp0 || !dp1) abort();
        dp0[0] = 1;               /* word "0" */
        dp1[0] = 1;               /* word "1" */
        cur_len = 1;              /* last processed row = 1 */

        /* Append a(1) = 3 (T(1,0) = 2*dp0[0] + dp1[0]) */
        append_flat((seq_t)dp0[0] * 2 + dp1[0]);   /* a(1) */
    }

    /* Generate rows until the requested index is available */
    while (flat_size <= n) {
        generate_next_row();
    }
    return flat[n];
}
