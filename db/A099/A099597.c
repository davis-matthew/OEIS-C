/*
 * A099597  Array T(n,k) read by antidiagonals: expansion of exp(x+y)/(1-xy).
 * https://oeis.org/A099597
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(0,0), T(0,1), T(1,0), T(0,2), T(1,1), T(2,0), ... *
 *            where T(n,k) satisfies T(0,k)=T(n,0)=1 and * T(n,k) =
 *            n*k*T(n-1,k-1)+1. The array is read by antidiagonals
 * Range:     u8 n <= 38, u16 n <= 70, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* cache of computed a(n) values */
static seq_t *seq = NULL;      /* array of sequence values */
static size_t seq_len = 0;     /* number of values stored */
static size_t seq_cap = 0;     /* allocated capacity of seq */

/* array of pointers to diagonals */
static seq_t **diags = NULL;   /* diags[d] -> array of length d+1 */
static size_t diag_cap = 0;    /* allocated capacity of diags */
static size_t current_max_d = 0;/* largest diagonal computed */

/* ----------------------------------------------------------------- */
/* Grow the sequence cache to hold at least 'need' elements.          */
static void grow_seq(size_t need)
{
    if (need <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < need)
        new_cap <<= 1;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* ----------------------------------------------------------------- */
/* Ensure that the diagonal array has space up to index 'd'.          */
static void ensure_diag_cap(size_t d)
{
    if (d < diag_cap)
        return;
    size_t new_cap = diag_cap ? diag_cap * 2 : 1;
    while (new_cap <= d)
        new_cap <<= 1;
    diags = (seq_t **)realloc(diags, new_cap * sizeof(seq_t *));
    diag_cap = new_cap;
}

/* ----------------------------------------------------------------- */
/* Compute the next diagonal (current_max_d + 1) and append its values. */
static void compute_next_diagonal(void)
{
    size_t d = current_max_d + 1;        /* next diagonal index */
    ensure_diag_cap(d);

    size_t len = d + 1;                  /* number of elements on this diagonal */
    seq_t *curr = (seq_t *)malloc(len * sizeof(seq_t));

    for (size_t i = 0; i <= d; ++i) {
        if (i == 0 || d - i == 0) {
            curr[i] = 1;
        } else {
            /* value from diagonal d-2 at position i-1 */
            seq_t prev_val = diags[d - 2][i - 1];
            curr[i] = (seq_t)i * (seq_t)(d - i) * prev_val + 1;
        }
    }

    diags[d] = curr;                     /* store the diagonal for future use */

    /* append to the antidiagonal sequence */
    grow_seq(seq_len + len);
    seq[seq_len++] = curr[0];
    for (size_t i = 1; i <= d; ++i)
        seq[seq_len++] = curr[i];

    current_max_d = d;
}

/* ----------------------------------------------------------------- */
/* Initialise data structures on the first call.                      */
static void init(void)
{
    if (seq != NULL)
        return;                         /* already initialised */

    /* sequence cache */
    grow_seq(16);
    seq_len = 1;
    seq[0] = 1;                         /* T(0,0) */

    /* diagonal tables */
    diag_cap = 1;
    diags = (seq_t **)malloc(sizeof(seq_t *));
    diags[0] = (seq_t *)malloc(sizeof(seq_t));  /* diagonal 0 */
    diags[0][0] = 1;
    current_max_d = 0;
}

/* ----------------------------------------------------------------- */
/* Return the n-th term of OEIS sequence A099597.                     */
seq_t a(idx_t n)
{
    init();

    /* produce values until we have at least (n+1) terms */
    while (seq_len <= (size_t)n)
        compute_next_diagonal();

    return seq[(size_t)n];
}
