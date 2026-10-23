/*
 * A009324  Expansion of e.g.f. log(1+log(1+x)/exp(x)).
 * https://oeis.org/A009324
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n! * [x^n] log(1 + log(1+x)/exp(x))
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 10, i64 n <= 17, i128 n <= 28,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:                                                        */
/*   P[n] = n! * p[n] = (-1)^{n+1} * Σ_{i=1..n} (n)_i / i           */
/*   A[0] = 0                                                          */
/*   For n ≥ 1:  A[n] = P[n] - Σ_{k=1}^{n-1} C(n-1,k-1) * P[n-k] * A[k]  */
/*   P[n] and C(n-1,k-1) are integers; all operations use only       */
/*   integer arithmetic, no fractions or floating point.              */

#include <stdint.h>
#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

static seq_t *P_array = NULL;        /* P[n] = n! * p[n]  */
static seq_t *A_array = NULL;        /* A[n] = a(n)        */
static size_t arr_size    = 0;       /* allocated size     */
static size_t computed_max = 0;       /* highest n already computed */

/* Ensure the arrays are large enough to hold indices up to 'need-1'   */
static void ensure_cache(size_t need)
{
    if (need <= arr_size) return;

    seq_t *newP = alloc_seq_array(need);
    seq_t *newA = alloc_seq_array(need);

    if (arr_size > 0) {
        for (size_t i = 0; i < arr_size; ++i) {
            newP[i] = P_array[i];
            newA[i] = A_array[i];
        }
    }

    if (P_array) free_seq_array(P_array);
    if (A_array) free_seq_array(A_array);

    P_array = newP;
    A_array = newA;
    arr_size = need;
}

/* Compute the sequence value a(n) for any n ≥ 0.                    */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    size_t N = (size_t)n;
    ensure_cache(N + 1);                /* indices 0..N */

    /* Compute terms up to N if not already computed.              */
    size_t start = (computed_max > 0) ? computed_max : 1;
    for (size_t i = start; i <= N; ++i)
    {
        /* ---------- compute P[i] ---------- */
        seq_t sum = (seq_t)0;
        seq_t term = (seq_t)i;           /* (i)_1 / 1 = i     */
        sum += term;
        for (size_t j = 2; j <= i; ++j)
        {
            /* term = term * (j-1) * (i-j+1) / j   */
            term = term * (seq_t)(j - 1);
            term = term * (seq_t)(i - j + 1);
            term = term / (seq_t)j;
            sum += term;
        }
        if (i % 2 == 0)                 /* even i → negative sign */
            P_array[i] = -sum;
        else
            P_array[i] = sum;

        /* ---------- compute A[i] ---------- */
        if (i == 1) {
            A_array[i] = P_array[i];
            continue;
        }
        seq_t binom = (seq_t)1;         /* C(i-1,0)   */
        seq_t sumA  = (seq_t)0;
        for (size_t k = 1; k <= i - 1; ++k)
        {
            seq_t termA = P_array[i - k];
            termA = termA * A_array[k];
            termA = termA * binom;
            sumA += termA;

            /* update binom: binom = binom * (i - k) / k = C(i-1,k)   */
            binom = binom * (seq_t)(i - k) / (seq_t)k;
        }
        A_array[i] = P_array[i] - sumA;
    }

    computed_max = N;
    return A_array[N];
}
