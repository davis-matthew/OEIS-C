/*
 * A099354  From P-positions in a certain game. Pile 1 tokens for select
 *          function (-)^y1 - (-)^x1 +3.
 * https://oeis.org/A099354
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = smallest non‑negative integer not occurring in the set *
 *            { a(0), b(0), a(1), b(1), …, a(n‑1), b(n‑1) } * Recurrence for
 *            b: * b(n) = b(n‑1) − a(n‑1) + a(n) + ( (b(n‑1) & 1) ? −1 : 1 ) *
 *            − ( (a(n‑1) & 1) ? −1 : 1 ) + 3 * with a(0) = 0, b(0) = 0
 * Range:     u8 n <= 195, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* global static storage */
static seq_t *A        = NULL;   /* sequence a */
static seq_t *B        = NULL;   /* sequence b */
static unsigned char *used = NULL;   /* marks whether an index has appeared */
static idx_t  used_cap  = 0;            /* capacity of 'used' */
static idx_t  A_cap    = 0;            /* capacity of A */
static idx_t  B_cap    = 0;            /* capacity of B */
static idx_t  computed = 0;            /* number of indices already computed */
static idx_t  min_candidate = 1;       /* next integer to test for a[i] */

/* Ensure 'arr' has at least 'need' elements;   'cap' is its current capacity. */
static void
ensure_seq_cap(seq_t **arr, idx_t *cap, idx_t need)
{
    if (need < *cap)
        return;
    idx_t new_cap = *cap ? *cap << 1 : 16;
    while (new_cap <= need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    *arr = tmp;
    *cap = new_cap;
}

/* Ensure 'used' has at least 'need' elements. */
static void
ensure_used_cap(idx_t need)
{
    if (need < used_cap)
        return;
    idx_t new_cap = used_cap ? used_cap << 1 : 16;
    while (new_cap <= need)
        new_cap <<= 1;
    unsigned char *tmp = (unsigned char *)realloc(used, new_cap * sizeof(unsigned char));
    if (!tmp)
        abort();
    /* zero the new cells */
    for (idx_t i = used_cap; i < new_cap; ++i)
        tmp[i] = 0;
    used = tmp;
    used_cap = new_cap;
}

/* Compute the n-th term of sequence a. */
seq_t a(idx_t n)
{
    /* First‑time initialization */
    if (!A) {
        A_cap = 16;
        A = (seq_t *)malloc(A_cap * sizeof(seq_t));
        if (!A)
            abort();

        B_cap = 16;
        B = (seq_t *)malloc(B_cap * sizeof(seq_t));
        if (!B)
            abort();

        used_cap = 16;
        used = (unsigned char *)calloc(used_cap, sizeof(unsigned char));
        if (!used)
            abort();

        A[0] = (seq_t)0;
        B[0] = (seq_t)0;
        used[0] = 1;      /* mark 0 as used */
        computed = 1;     /* a(0) already computed */
        min_candidate = 1;/* next available integer */
    }

    /* Compute all missing indices up to n */
    for (idx_t i = computed; i <= n; ++i) {
        /* Find the smallest integer not yet used */
        while (1) {
            ensure_used_cap(min_candidate);
            if (!used[min_candidate])
                break;
            ++min_candidate;
        }

        /* Allocate space for new entries */
        ensure_seq_cap(&A, &A_cap, i);
        ensure_seq_cap(&B, &B_cap, i);

        /* Set a[i] and mark it used */
        seq_t curr_a = (seq_t)min_candidate;
        A[i] = curr_a;
        used[min_candidate] = 1;
        ++min_candidate;          /* advance candidate for next discovery */

        /* Compute b[i] using the recurrence */
        seq_t prev_a = A[i-1];
        seq_t prev_b = B[i-1];
        seq_t term = prev_b - prev_a + curr_a
                   + ((prev_b & 1) ? (seq_t)(-1) : (seq_t)1)
                   - ((prev_a & 1) ? (seq_t)(-1) : (seq_t)1)
                   + (seq_t)3;
        B[i] = term;

        /* Mark b[i] as used if it is non‑negative and fits in idx_t */
        if (term >= (seq_t)0 && term <= (seq_t)(((idx_t)-1))) {
            idx_t b_idx = (idx_t)term;
            ensure_used_cap(b_idx);
            used[b_idx] = 1;
        }
    }

    computed = n + 1;
    return A[n];
}
