/*
 * A167550  The a(n,n+1) diagonal of the ED1 array A167546.
 * https://oeis.org/A167550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*(n-1)*a(n-1)+3*(n-1)! (a(1)=1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


static seq_t *A = NULL;
static idx_t A_size = 0;
static idx_t A_max = 0;

static seq_t *fact = NULL;
static idx_t fact_size = 0;
static idx_t fact_max = 0;

static void ensure_cap(idx_t *size, seq_t **arr, idx_t needed)
{
    if (*size < needed) {
        seq_t *tmp = (seq_t *)realloc(*arr, sizeof(seq_t) * needed);
        if (!tmp) abort();
        *arr = tmp;
        *size = needed;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* Undefined for n=0, return 0 */

    if (A == NULL) {
        A_size = 2;
        A = (seq_t *)malloc(sizeof(seq_t) * A_size);
        if (!A) abort();
        A[0] = (seq_t)0;
        A[1] = (seq_t)1;
        A_max = 1;
    }

    if (fact == NULL) {
        fact_size = 1;
        fact = (seq_t *)malloc(sizeof(seq_t) * fact_size);
        if (!fact) abort();
        fact[0] = (seq_t)1;
        fact_max = 0;
    }

    if (n <= A_max) return A[n];

    ensure_cap(&A_size, &A, n + 1);
    ensure_cap(&fact_size, &fact, n);

    if ((idx_t)(n - 1) > fact_max) {
        for (idx_t j = fact_max + 1; j <= n - 1; ++j)
            fact[j] = fact[j - 1] * (seq_t)j;
        fact_max = n - 1;
    }

    for (idx_t i = A_max + 1; i <= n; ++i) {
        seq_t term = A[i - 1] * ((seq_t)4 * (seq_t)(i - 1));
        A[i] = term + (seq_t)3 * fact[i - 1];
    }

    A_max = n;
    return A[n];
}
