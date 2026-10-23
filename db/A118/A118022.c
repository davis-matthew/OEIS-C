/*
 * A118022  Triangle T, read by rows, such that T^2 = SHIFT-UP(T); i.e., the
 *          matrix square of T shifts each column of T up 1 row, dropping the
 *          main diagonal consisting of the powers of 2: [T^2](n,k) = T(n+1,k)
 *          with T(n,n) = 2^n for n>=k>=0.
 * https://oeis.org/A118022
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) with r = ⌊(√(8n+1)-1)/2⌋, k = n - r(r+1)/2 where
 *            T(r,k) = A118023(r-k)·2^(k·(r-k+1)) A118023 is the column‑0
 *            sequence of the triangle T: A[0] = 1 A[n+1] = Σ_{i=0..n}
 *            A[i]·A[n-i]·2^(i·(n+1-i)) for n≥0 The code below memoises A[·]
 *            and evaluates the formula
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 35, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/pow2.h"

/* Memoisation storage for A118023.  The implementation uses
   new/delete when compiled as C++ and malloc/realloc otherwise. */
static seq_t *A = NULL;
static size_t Acap = 0;      /* capacity of A */
static idx_t Amax = 0;       /* last index that has been initialised */
#ifdef __cplusplus
/* Helper: allocation with C++ new/delete. */
static void ensure_capacity(size_t needed)
{
    if (!A) {
        A = new seq_t[needed];
        Acap = needed;
    } else if (Acap < needed) {
        seq_t *tmp = new seq_t[needed];
        for (size_t i = 0; i < Acap; ++i) tmp[i] = A[i];
        delete[] A;
        A = tmp;
        Acap = needed;
    }
}
#else
/* Helper: allocation with malloc/realloc. */
static void ensure_capacity(size_t needed)
{
    if (!A) {
        A = (seq_t*)malloc(needed * sizeof(seq_t));
        Acap = needed;
    } else if (Acap < needed) {
        seq_t *tmp = (seq_t*)realloc(A, needed * sizeof(seq_t));
        A = tmp;
        Acap = needed;
    }
}
#endif

/* Compute A118023 up to index target (inclusive). */
static void ensure_A(idx_t target)
{
    size_t needed = (size_t)target + 1;
    ensure_capacity(needed);

    /* Initialise base case only once. */
    static bool initialized = false;
    if (!initialized) {
        A[0] = (seq_t)1;
        Amax = 0;
        initialized = true;
    }

    for (idx_t n = Amax + 1; n <= target; ++n) {
        seq_t sum = 0;
        for (idx_t i = 0; i < n; ++i) {
            seq_t prod = A[i] * A[n - 1 - i];
            prod = prod * pow2((idx_t)(i * (n - i)));
            sum += prod;
        }
        A[n] = sum;
    }
    Amax = target;
}

/* Return the row number r such that r(r+1)/2 <= n < (r+1)(r+2)/2. */
static idx_t row_of(idx_t n)
{
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n) ++r;
    return r;
}

/* Entry point for the sequence A118022. */
seq_t a(idx_t n)
{
    idx_t r  = row_of(n);
    idx_t tri = (r * (r + 1)) / 2;
    idx_t k  = n - tri;          /* column index   */
    idx_t m  = r - k;            /* index for A[m] */

    /* Ensure A[m] is available. */
    ensure_A(m);

    /* Compute power of two factor. */
    idx_t exp = k * (m + 1);
    seq_t pow = pow2(exp);

    return A[m] * pow;
}
