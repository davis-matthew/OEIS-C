/*
 * A215625  Expansion of q^(2/3) * c(q) / c(q^3) in powers of q where c() is a
 *          cubic AGM theta function.
 * https://oeis.org/A215625
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 18, i16 n <= 162, i32 n <= 338, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A215625 – coefficients of q^(2/3)*c(q)/c(q^3)
 *
 * Recurrence:
 *   a(0) = 1
 *   Let b_k be the 9‑periodic sequence
 *     {1, 1, -3, 1, 1, -3, 1, 1, 0}  (index k = 1,2,…).
 *   Define c_k = Σ_{d|k} d * b_d
 *   (the sum runs over all positive divisors d of k).
 *   Then for n ≥ 1:
 *     a(n) = (1/n) * Σ_{k=1}^n c_k * a(n – k).
 *
 * The implementation memoises the computed values so that arbitrary
 * order of calls is supported.  It works with any integer type
 * used for seq_t (built‑in or arbitrary‑precision).  No floating point
 * arithmetic is used.
 */

#include <stdlib.h>
#include <stdint.h>

/* period‑9 sequence b(k) for k = 1,2,… */
static const int8_t B[9] = { 1, 1, -3, 1, 1, -3, 1, 1, 0 };

/* memoised arrays */
static seq_t *A = NULL;      /* a(n) for 0 ≤ n < AC */
static seq_t *C = NULL;      /* c_n for 1 ≤ n < AC */
static size_t AC = 0;        /* current capacity of A and C */
static idx_t cur_max = 0;    /* highest n that has been computed */

/* recompute c_k for all 1 ≤ k < up */
/* C array must be allocated to length ≥ up */
static void compute_C(size_t up)
{
    for (idx_t k = 1; (size_t)k < up; ++k) {
        seq_t sum = 0;
        for (idx_t d = 1; d * d <= k; ++d) {
            if (k % d == 0) {
                int8_t v1 = B[(d - 1) % 9];
                sum += (seq_t)d * (seq_t)v1;
                idx_t d2 = k / d;
                if (d2 != d) {
                    int8_t v2 = B[(d2 - 1) % 9];
                    sum += (seq_t)d2 * (seq_t)v2;
                }
            }
        }
        C[k] = sum;
    }
}

/* Ensure that arrays have room for indices 0 … n
 * and that a(0)…a(n) are computed.
 */
static void ensure_capacity(idx_t n)
{
    if (n <= cur_max) return;

    /* grow array: at least n+1 elements needed */
    size_t new_cap = AC ? AC : 1;
    while (new_cap <= (size_t)(n + 1)) new_cap <<= 1;

    A = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
    C = (seq_t *)realloc(C, new_cap * sizeof(seq_t));
    AC = new_cap;

    /* compute c_k for new capacity */
    compute_C(AC);

    /* compute a(n) for newly needed indices */
    for (idx_t i = cur_max + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 1; k <= i; ++k) {
            sum += C[k] * A[i - k];
        }
        A[i] = sum / (seq_t)i;
    }
    cur_max = n;
}

seq_t a(idx_t n)
{
    /* initialize on first call */
    if (A == NULL) {
        AC = 1;
        A = (seq_t *)malloc(AC * sizeof(seq_t));
        C = (seq_t *)malloc(AC * sizeof(seq_t));
        A[0] = (seq_t)1;      /* a(0) = 1 */
        cur_max = 0;
    }

    ensure_capacity(n);
    return A[n];
}
