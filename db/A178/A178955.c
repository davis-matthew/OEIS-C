/*
 * A178955  E.g.f. is inverse of e.g.f. for Catalan numbers.
 * https://oeis.org/A178955
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 15, i64 n <= 24, i128 n <= 29,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recursive: a(0)=1; a(n) = -sum_{k=1}^n binomial(n,k)*Catalan(k)*a(n-k) */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/catalan.h"

static seq_t *A = NULL;        /* array of a(n) values */
static idx_t capA = 0;         /* current allocated size of A   */
static idx_t computedA = 0;    /* highest index in A already computed */

static seq_t *cat = NULL;      /* array of Catalan numbers up to n */
static idx_t capC = 0;         /* current allocated size of cat */
static idx_t computedC = 0;    /* highest index in cat already computed */

/* ensure that array *arr has size at least 'needed'  */
static void grow_seq_array(seq_t **arr, idx_t *cap, idx_t needed)
{
    if (needed <= *cap)
        return;
    seq_t *newarr = (seq_t*)malloc(needed * sizeof(seq_t));
    if (*cap) {
        memcpy(newarr, *arr, (*cap) * sizeof(seq_t));
        free(*arr);
    }
    *arr  = newarr;
    *cap  = needed;
}

seq_t a(idx_t n)
{
    /* grow the a(n) array if necessary */
    if (n + 1 > capA)
        grow_seq_array(&A, &capA, n + 1);

    /* ensure Catalan numbers are available up to n */
    if (n + 1 > capC)
        grow_seq_array(&cat, &capC, n + 1);
    for (idx_t i = computedC; i <= n; ++i)
        cat[i] = catalan(i);
    computedC = n + 1;

    /* compute a(k) for any k > computedA up to n */
    for (idx_t m = computedA; m <= n; ++m) {
        if (m == 0) {
            A[0] = 1;   /* a(0) = 1 */
            continue;
        }
        seq_t sum = 0;
        seq_t binom_val = 1;     /* binomial(m,0) */
        for (idx_t k = 1; k <= m; ++k) {
            binom_val = binom_val * (seq_t)(m - k + 1) / (seq_t)k;
            seq_t term = binom_val * cat[k] * A[m - k];
            sum += term;
        }
        A[m] = -sum;   /* a(m) = -sum_{k=1..m} binom(m,k)*cat[k]*a(m-k) */
    }
    computedA = n;
    return A[n];
}
