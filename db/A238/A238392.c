/*
 * A238392  Triangle read by rows: each row is an initial segment of the terms
 *          of A000123 followed by its reflection.
 * https://oeis.org/A238392
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   A(0)=1; for n>0, A(n)=∑_{k=0}^{n} A(min(k,n-k)) a(n) is the n-th
 *            element of the triangle read by rows: the r-th row is the
 *            initial segment of A followed by its reflection, where r
 *            satisfies r(r+1)/2 ≤ n < (r+1)(r+2)/2 and k=n−r(r+1)/2.
 *            T(r,k)=A(min(k,r−k))
 * Range:     u8 n <= 683, u16 n <= 8000, u32 n <= 8000, u64 n <= 8000,
 *            u128 n <= 8000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard includes */
#include <stdint.h>
#include <stdlib.h>

/* ----------  Memoised computation of A(n)  ---------- */
static seq_t *A = NULL;    /* values of A(n)                */
static seq_t *P = NULL;    /* prefix sums P[i] = ∑_{t=0}^{i} A(t) */
static size_t A_cap = 0;   /* current capacity of A & P     */

/* Expand the arrays so that index 'needed-1' is valid. */
static void ensure_A(size_t needed)
{
    if (needed <= A_cap) return;          /* already computed */

    /* Determine new capacity (double each time). */
    size_t new_cap = A_cap ? A_cap * 2 : 4;
    while (new_cap < needed) new_cap *= 2;

    /* Reallocate the arrays. */
    seq_t *new_A = (seq_t*)realloc(A, new_cap * sizeof(seq_t));
    seq_t *new_P = (seq_t*)realloc(P, new_cap * sizeof(seq_t));
    if (!new_A || !new_P) exit(1);          /* allocation error */

    A = new_A;
    P = new_P;

    /* Compute new values.  Start from the first missing index. */
    if (A_cap == 0) {                 /* initialise the very first term */
        A[0] = (seq_t)1;
        P[0] = A[0];
    }

    for (size_t i = A_cap; i < new_cap; ++i) {
        if (i == 0) continue;          /* already initialised */

        seq_t sum_before;
        if (i % 2 == 0) {               /* even index i = 2*m */
            size_t m = i / 2;
            sum_before = (m == 0) ? (seq_t)0 : P[m - 1];
            seq_t double_sum = sum_before + sum_before;
            A[i] = double_sum + A[m];
        } else {                        /* odd index i = 2*m+1 */
            size_t m = i / 2;            /* floor(i/2) */
            sum_before = P[m];
            seq_t double_sum = sum_before + sum_before;
            A[i] = double_sum;
        }
        P[i] = P[i - 1] + A[i];
    }

    A_cap = new_cap;
}

/* ----------  Find the row index r  ---------- */
static idx_t find_row(idx_t n)
{
    idx_t low = 0;
    idx_t high = n + 1;                /* r never exceeds n   */
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        __uint128_t tri = (__uint128_t)mid * (mid + 1) / 2;
        if (tri <= n)
            low = mid;
        else
            high = mid;
    }
    return low;
}

/* ----------  Public function  ---------- */
seq_t a(idx_t n)
{
    /* Determine which row and which position within that row. */
    idx_t r = find_row(n);
    __uint128_t before128 = (__uint128_t)r * (r + 1) / 2;
    idx_t before = (idx_t)before128;
    idx_t k = n - before;
    idx_t idx = (k <= r - k) ? k : (r - k);     /* min(k, r−k) */

    /* Make sure the A array contains A[idx]. */
    ensure_A((size_t)idx + 1);

    return A[idx];
}
