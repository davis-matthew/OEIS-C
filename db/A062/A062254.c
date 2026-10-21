/*
 * A062254  3rd level triangle related to Eulerian numbers and binomial
 *          transforms (A062253 is second level, triangle of Eulerian numbers
 *          is first level and triangle with Z(0,0)=1 and Z(n,k)=0 otherwise
 *          is 0th level).
 * https://oeis.org/A062254
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 28, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A062254 – 3rd level triangle (row‑major concatenation)
    Recurrence:
        A1(n,k) = (k+1)A1(n‑1,k) + (n‑k)A1(n‑1,k‑1)      // Eulerian numbers
        A2(n,k) = (k+2)A2(n‑1,k) + (n‑k)A2(n‑1,k‑1) + A1(n,k)
        A3(n,k) = (k+3)A3(n‑1,k) + (n‑k)A3(n‑1,k‑1) + A2(n,k)
    a(n) is the n‑th element of the row‑major sequence formed by A3.
*/

#include <stdlib.h>

/* Global tables holding the current row of each level and the
   concatenated sequence of the third level.  The types seq_t and
   idx_t are supplied by the build system, so no extern declaration
   is required. */
static seq_t *a3_concat = NULL;     /* concatenated A3 values */
static size_t a3_cap = 0;           /* allocated capacity of a3_concat   */
static idx_t  a3_count = 0;         /* number of values stored in a3_concat */

static seq_t *prevA1 = NULL;        /* previous row of A1 (Eulerian)   */
static seq_t *prevA2 = NULL;        /* previous row of A2              */
static seq_t *prevA3 = NULL;        /* previous row of A3              */
static idx_t  prev_len = 0;         /* length of the previous row      */
static idx_t  last_row = (idx_t)-1; /* index of last built row         */

/* Ensure that a3_concat has room for at least 'needed' elements. */
static void ensure_a3_capacity(size_t needed)
{
    if (needed <= a3_cap) return;
    size_t new_cap = a3_cap ? a3_cap * 2 : 8;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(a3_concat, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* out of memory – terminate program */
    a3_concat = tmp;
    a3_cap = new_cap;
}

/* Initialise the first row (row 0). */
static void init_once(void)
{
    if (a3_count != 0) return;     /* already initialised */

    prev_len = 1;
    prevA1 = (seq_t *)malloc(prev_len * sizeof(seq_t));
    prevA2 = (seq_t *)malloc(prev_len * sizeof(seq_t));
    prevA3 = (seq_t *)malloc(prev_len * sizeof(seq_t));

    prevA1[0] = 1;    /* A1(0,0) */
    prevA2[0] = 1;    /* A2(0,0) */
    prevA3[0] = 1;    /* A3(0,0) */

    ensure_a3_capacity(1);
    a3_concat[0] = 1;   /* first element of the concatenated sequence */
    a3_count = 1;
    last_row = 0;
}

/* Build the next row of all three triangles. */
static void build_next_row(void)
{
    idx_t m = last_row + 1;        /* new row index */
    idx_t new_len = m + 1;         /* length of the new row */

    seq_t *newA1 = (seq_t *)calloc(new_len, sizeof(seq_t));
    seq_t *newA2 = (seq_t *)calloc(new_len, sizeof(seq_t));
    seq_t *newA3 = (seq_t *)calloc(new_len, sizeof(seq_t));

    for (idx_t k = 0; k < new_len; ++k) {
        /* --- Level 1: Eulerian numbers A1 --- */
        seq_t term1 = 0, term2 = 0;
        if (k <= m - 1) term1 = ((seq_t)(k + 1)) * prevA1[k];
        if (k > 0)      term2 = ((seq_t)(m - k)) * prevA1[k - 1];
        seq_t a1_cur = term1 + term2;
        newA1[k] = a1_cur;

        /* --- Level 2: A2 --- */
        seq_t t1 = 0, t2 = 0;
        if (k <= m - 1) t1 = ((seq_t)(k + 2)) * prevA2[k];
        if (k > 0)      t2 = ((seq_t)(m - k)) * prevA2[k - 1];
        seq_t a2_cur = t1 + t2 + a1_cur;
        newA2[k] = a2_cur;

        /* --- Level 3: A3 --- */
        seq_t s1 = 0, s2 = 0;
        if (k <= m - 1) s1 = ((seq_t)(k + 3)) * prevA3[k];
        if (k > 0)      s2 = ((seq_t)(m - k)) * prevA3[k - 1];
        seq_t a3_cur = s1 + s2 + a2_cur;
        newA3[k] = a3_cur;
    }

    /* Append new row to the global concatenated array. */
    ensure_a3_capacity(a3_count + (size_t)new_len);
    for (idx_t k = 0; k < new_len; ++k)
        a3_concat[a3_count + k] = newA3[k];

    a3_count += new_len;

    /* Free the old row arrays. */
    free(prevA1);
    free(prevA2);
    free(prevA3);

    /* Promote the new row to “previous” for the next iteration. */
    prevA1 = newA1;
    prevA2 = newA2;
    prevA3 = newA3;

    prev_len = new_len;
    last_row = m;
}

/* Public entry point: a(n) – concatenated 3rd‑level triangle. */
seq_t a(idx_t n)
{
    init_once();                         /* ensure row 0 is available */
    while (a3_count <= n) build_next_row();  /* build until we have n+1 elements */
    return a3_concat[n];
}
