/*
 * A082170  Deterministic completely defined quasi-acyclic automata with 3
 *          inputs, n transient and k absorbing labeled states.
 * https://oeis.org/A082170
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 52, u128 n <= 117,
 *            big any n
 * Validated: standard, pass, 52 terms through n = 1174 (2026-11-01)
 * Helpers:   binomial, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A082170: Deterministic completely defined quasi‑acyclic automata
 * with 3 inputs, n transient and k absorbing labeled states.
 * Recurrence:
 *   T(0,k)   = 1
 *   T(n,k)   = Σ_{i=0}^{n-1} (-1)^(n-i-1) * binomial(n,i) *
 *              (i+k)^(3*(n-i)) * T(i,k)      (n > 0)
 * Sequence indexing : read T(n,k) by anti‑diagonals.
 *   a(0) = T(0,1)
 *   a(1) = T(0,2)   …   a(N) = T(n,k) where (n,k) are the coordinates
 *   of the N‑th anti‑diagonal element.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/ipow.h"

/* map global index N to the anti‑diagonal coordinates (n,k) */
static void idx_to_coords(idx_t N, idx_t *n, idx_t *k)
{
    idx_t d = 1;   /* diagonal number (1‑based) */
    while (true) {
        idx_t sum_prev = (d - 1) * d / 2;
        if (N < sum_prev + d) break;
        ++d;
    }
    idx_t sum_prev = (d - 1) * d / 2;
    idx_t p        = N - sum_prev;      /* position on diagonal, 0‑based */
    *n = p;                            /* transient count */
    *k = d - p;                        /* absorbing labelled states */
}

#define MAX_NK 200          /* sufficient for n,k up to ~150 (index<10 000) */

/* memoisation tables for T(n,k) */
static seq_t  T[MAX_NK][MAX_NK];
static bool   computed[MAX_NK][MAX_NK];

/* forward declaration */
static seq_t get_T(idx_t n, idx_t k);

/* compute T(n,k) with memoisation */
static seq_t get_T(idx_t n, idx_t k)
{
    if (computed[n][k])         return T[n][k];
    if (n == 0) {               /* base case */
        T[n][k] = 1;
        computed[n][k] = true;
        return 1;
    }

    seq_t pos = 0;   /* Σ of terms with positive sign */
    seq_t neg = 0;   /* Σ of terms with negative sign */

    for (idx_t i = 0; i < n; ++i) {
        seq_t bin   = binomial(n, i);                     /* binomial(n,i) */
        seq_t expct = (seq_t) (3 * (n - i));              /* 3*(n-i)   */
        seq_t pw    = ipow((seq_t)(i + k), expct);
        seq_t term  = bin * pw;
        term *= get_T(i, k);

        if (((n - i - 1) & 1) != 0)  /* odd => negative sign */
            neg += term;
        else
            pos += term;
    }

    seq_t result = pos - neg;   /* pos ≥ neg by definition */
    T[n][k] = result;
    computed[n][k] = true;
    return result;
}

/* public sequence function */
seq_t a(idx_t N)
{
    idx_t n, k;
    idx_to_coords(N, &n, &k);
    return get_T(n, k);
}
