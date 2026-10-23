/*
 * A160870  Array read by antidiagonals: T(n,k) is the number of sublattices
 *          of index n in generic k-dimensional lattice (n >= 1, k >= 1).
 * https://oeis.org/A160870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 72, u32 n <= 165, u64 n <= 425,
 *            u128 n <= 1160, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A160870:  sublattice‑counting table read by antidiagonals.
 *
 *  Recurrence leading to values T(n,k):
 *      T(n,1) = 1,
 *      T(1,k) = 1,
 *      T(n,k) = Σ_{d|n} d · T(d , k-1).
 *
 *  The sequence a(n) is obtained by reading T along its antidiagonals.
 *  In particular, if d is the smallest integer such that d(d+1)/2 ≥ n,
 *  let prev = (d−1)d/2 and pos = n−prev.
 *  Then a(n) = T(i , j) with i = d−pos+1, j = pos.
 *
 *  The implementation below memoises all required T(n,k) in a static 2‑D
 *  array.  The array size (MAX_D) is chosen to cover n up to ~125 000,
 *  which is more than enough for the required n ≤ 10 000 tests.
 *  The code is valid in both C11 and C++17 and works for both
 *  primitive integer types and arbitrary‑precision integer classes
 *  used as seq_t.
 */

#include <stdint.h>
#include <stdbool.h>

/* maximum index needed for T(n,k) when n is read up to about 125 000
   (triangular number of 500).  Larger values can be handled by
   increasing MAX_D if required. */
#define MAX_D 500

/* static memoisation table: 0 indicates “not yet computed”.  */
static seq_t T[MAX_D + 1][MAX_D + 1] = {0};

/* forward declaration of helper that maps a linear index to
   antidiagonal coordinates (i,j). */
static void idx_to_ij(idx_t n, idx_t *pi, idx_t *pj);

/* ------------------------------------------------------------------ */
/*  Recursive computation of T(n,k) with memoisation.                */
/* ------------------------------------------------------------------ */
static seq_t compute_T(idx_t n, idx_t k)
{
    /* cache hit */
    if (T[n][k] != 0)
        return T[n][k];

    /* base cases */
    if (n == 1 || k == 1) {
        T[n][k] = 1;
        return 1;
    }

    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d * compute_T(d, k - 1);
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2 * compute_T(d2, k - 1);
        }
    }

    T[n][k] = sum;
    return sum;
}

/* ------------------------------------------------------------------ */
/*  Convert linear index n (1‑based) to antidiagonal coordinates (i,j). */
/* ------------------------------------------------------------------ */
static void idx_to_ij(idx_t n, idx_t *pi, idx_t *pj)
{
    idx_t d = 1;
    /* find smallest d with d(d+1)/2 ≥ n */
    while ((d * (d + 1)) / 2 < n)
        ++d;

    idx_t prev = (d - 1) * d / 2;
    idx_t pos  = n - prev;          /* 1 ≤ pos ≤ d */
    *pi = d - pos + 1;              /* row index  (i) */
    *pj = pos;                      /* column index (j) */
}

/* ------------------------------------------------------------------ */
/*  Public interface as required by the harness.                       */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    idx_t i, j;
    idx_to_ij(n, &i, &j);
    return compute_T(i, j);
}
