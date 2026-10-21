/*
 * A063299  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          26 ).
 * https://oeis.org/A063299
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-4) + a(n-6) - a(n-10) for n>13 initial values: a(2) =
 *            -1; a(3) = 6; a(4) = 14; a(5) = 21; a(6) = 28; a(7) = 35; a(8) =
 *            42; a(9) = 47; a(10) = 56; a(11) = 61; a(12) = 70; a(13) = 77;
 * Range:     i8 n <= 20, i16 n <= 53, i32 n <= 53, i64 n <= 53, i128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A063299 (offset 2). */
#include <stdlib.h>

static seq_t *A = NULL;      /* dynamic array holding the sequence */
static size_t cap = 0;       /* current capacity of A */
static size_t len = 0;       /* number of valid entries in A */

/* Ensure the internal array can hold indices up to n. */
static void ensure_size(idx_t n) {
    if ((size_t)(n + 1) <= cap)
        return;
    size_t newcap = (cap == 0) ? 32 : cap * 2;
    while (newcap < (size_t)(n + 1))
        newcap *= 2;
    seq_t *tmp = (seq_t *)realloc(A, newcap * sizeof(seq_t));
    if (!tmp) { /* allocation failure – undefined behaviour in this context */
        exit(1);
    }
    A = tmp;
    /* zero the newly allocated part – not strictly necessary but safe */
    for (size_t i = cap; i < newcap; ++i)
        A[i] = 0;
    cap = newcap;
}

/* Small helper to initialise the base terms for n <= 13. */
static void init_base(void) {
    A[0] = 0; A[1] = 0;                     /* values for indices < 2 are irrelevant */
    /* a(2) ... a(13) */
    const seq_t init[12] = {
        -1, 6, 14, 21, 28, 35, 42, 47, 56, 61, 70, 77
    };
    for (int i = 2; i <= 13; ++i)
        A[i] = init[i - 2];
    len = 14;  /* we have filled indices 0..13 */
}

seq_t a(idx_t n) {
    if (n < 2)
        return 0;   /* behaviour for n < 2 is unspecified; return 0 */

    ensure_size(n);

    /* Initialise base values if not done yet */
    if (len == 0)
        init_base();

    /* Compute missing values up to n */
    for (idx_t i = (idx_t)len; i <= n; ++i) {
        /* For i <= 13 the values are already set, so skip the recurrence */
        if (i > 13) {
            A[i] = A[i - 4] + A[i - 6] - A[i - 10];
        }
    }
    len = (size_t)n + 1;
    return A[n];
}
