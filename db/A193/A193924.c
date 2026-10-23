/*
 * A193924  Mirror of the triangle A193923.
 * https://oeis.org/A193924
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A193924 – Mirror of the triangle A193923.
 * Triangle w(n,k):
 *   w(n,0)   = 1                     for all n
 *   w(n,n)   = Fibonacci(2*n)        (F(0)=0,F(1)=1)
 *   w(n,k)   = w(n-1,k-1) + w(n-1,k)  for 1 ≤ k < n
 * The sequence a(n) is the flattened list obtained by reversing each
 * row of w before concatenation.
 */

#include <stdlib.h>   /* malloc, realloc, exit */

/* ----- Static storage used by the routine ------------------------------ */
static seq_t *seq      = NULL;   /* flattened sequence */
static size_t seq_cap  = 0;
static size_t seq_len  = 0;      /* number of valid elements in seq */

/* Row buffers – alternating between two rows */
static seq_t *row_a     = NULL;  /* current row (ascending order) */
static size_t row_a_cap = 0;
static size_t row_a_len = 0;     /* length of current row */

static seq_t *row_b     = NULL;  /* buffer for the next row */
static size_t row_b_cap = 0;
static size_t row_b_len = 0;     /* length of new row */

/* Fibonacci numbers (fib[i] = F(i)) */
static seq_t *fib      = NULL;
static size_t fib_cap  = 0;
static size_t fib_len  = 0;

/* ----- Helper functions ---------------------------------------------- */
static void grow_array(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap > 0) ? (*cap << 1) : 16;
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);   /* abort on allocation failure */
        *arr = tmp;
        *cap = new_cap;
    }
}

static void push_seq(seq_t val)
{
    grow_array(&seq, &seq_cap, seq_len + 1);
    seq[seq_len++] = val;
}

static void ensure_fib(idx_t idx)
{
    /* Fill fib[] up to and including fib[idx] */
    while (fib_len <= idx) {
        /* For idx 0 or 1, the loop will not execute because fib_len>=2 initially. */
        seq_t next = fib[fib_len - 1] + fib[fib_len - 2];
        grow_array(&fib, &fib_cap, fib_len + 1);
        fib[fib_len++] = next;
    }
}

/* ----- Main routine ---------------------------------------------------- */
seq_t a(idx_t n)
{
    /* --- Initialisation on first call -------------------------------- */
    if (seq_len == 0) {
        /* First row (row 0) */
        grow_array(&row_a, &row_a_cap, 1);
        row_a_len = 1;
        row_a[0] = (seq_t)1;

        /* Initial flattened sequence: just the first element */
        grow_array(&seq, &seq_cap, 1);
        seq_len = 1;
        seq[0]  = (seq_t)1;

        /* Fibonacci buffer – compute F(0) and F(1) */
        grow_array(&fib, &fib_cap, 2);
        fib[0] = (seq_t)0;
        fib[1] = (seq_t)1;
        fib_len = 2;
    }

    /* --- Generate rows until we have enough terms --------------------- */
    while (seq_len <= (size_t)n) {
        /* Current row number (r) equals the length of row_a */
        idx_t r = (idx_t)row_a_len;
        idx_t new_len = row_a_len + 1;   /* length of the next row */

        /* Prepare buffer for the new row */
        grow_array(&row_b, &row_b_cap, new_len);
        row_b_len = new_len;
        row_b[0] = (seq_t)1;            /* w(r,0) = 1 */

        /* Interior entries: Pascal's rule */
        for (idx_t k = 1; k + 1 < new_len; ++k)
            row_b[k] = row_a[k - 1] + row_a[k];

        /* Last entry */
        ensure_fib(2 * r);              /* need Fib(2*r) */
        row_b[new_len - 1] = fib[2 * r];

        /* Append the row in reverse order to the flattened sequence */
        for (idx_t k = new_len; k > 0; --k)
            push_seq(row_b[k - 1]);

        /* Swap the buffers for the next iteration */
        seq_t *tmp  = row_a;
        row_a       = row_b;
        row_b       = tmp;
        row_a_len   = new_len;
        /* row_b_len will be overwritten in the next iteration */
    }

    return seq[(size_t)n];
}
