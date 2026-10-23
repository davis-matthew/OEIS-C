/*
 * A143138  E.g.f.: A(x) = x + (exp(A(x)) - 1)^2.
 * https://oeis.org/A143138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of labeled structures defined by A(x) = x +
 *            (exp(A(x)) - 1)^2 (EGF)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Auxiliary function to ensure a dynamic array has enough capacity */
static void ensure_capacity(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        *cap = need * 2;
        *arr = (seq_t *)realloc(*arr, *cap * sizeof(seq_t));
    }
}

/* Global storage for the computed terms */
static seq_t *A_arr = NULL;          /* a[n]          */
static size_t A_cap = 0, A_len = 0;
static seq_t *E_arr = NULL;          /* e[n] (exp(A) coeff)   */
static size_t E_cap = 0, E_len = 0;
static seq_t *G_arr = NULL;          /* g[n] = e[n] for n>=1   */
static size_t G_cap = 0, G_len = 0;

/* Initialisation of the sequence tables */
static void init(void)
{
    if (A_arr != NULL)
        return;                     /* already initialised */

    /* Allocate for A, E, G with some starting capacity */
    ensure_capacity(&A_arr, &A_cap, 8);
    ensure_capacity(&E_arr, &E_cap, 8);
    ensure_capacity(&G_arr, &G_cap, 8);

    /* a[0] = 0 (unused), a[1] = 1 */
    A_arr[0] = (seq_t)0;
    A_arr[1] = (seq_t)1;
    A_len = 2;

    /* e[0] = 1, e[1] = a[1] */
    E_arr[0] = (seq_t)1;
    E_arr[1] = A_arr[1];
    E_len = 2;

    /* g[0] unused, g[1] = e[1] */
    G_arr[0] = (seq_t)0;
    G_arr[1] = E_arr[1];
    G_len = 2;
}

/* Main function computing the n-th term of the sequence */
seq_t a(idx_t n)
{
    init();

    if (n == 0)         /* offset is 1, but just in case */
        return (seq_t)0;

    size_t N = (size_t)n;

    /* Compute all terms up to N if not already done */
    for (size_t i = A_len; i <= N; ++i) {
        /* ---- Compute a[i] ---- */
        seq_t sumA = 0;
        seq_t binom = 1;                    /* binom(i , 0) */
        for (size_t k = 1; k < i; ++k) {
            binom = binom * (seq_t)(i - k + 1) / (seq_t)k; /* binom(i, k) */
            /* G indices are 1..i-1 */
            sumA += binom * G_arr[k] * G_arr[i - k];
        }
        ensure_capacity(&A_arr, &A_cap, A_len + 1);
        A_arr[A_len++] = sumA;

        /* ---- Compute e[i] (coeff of exp(A)) ---- */
        seq_t sumE = 0;
        seq_t binomE = 1;                   /* binom(i-1, 0) */
        for (size_t k = 1; k <= i; ++k) {
            sumE += binomE * E_arr[i - k] * A_arr[k];
            if (k < i)
                binomE = binomE * (seq_t)(i - k) / (seq_t)k; /* next binom */
        }
        ensure_capacity(&E_arr, &E_cap, E_len + 1);
        E_arr[E_len++] = sumE;

        /* ---- g[i] = e[i] for i>=1 ---- */
        ensure_capacity(&G_arr, &G_cap, G_len + 1);
        G_arr[G_len++] = sumE;
    }

    return A_arr[N];
}
