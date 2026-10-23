/*
 * A101030  Triangle read by rows: T(n,k) = number of functions from an
 *          n-element set into but not onto a k-element set.
 * https://oeis.org/A101030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 26, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   pow_seq_idx, triangular
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle read by rows (OEIS A101030):
   a(n) = T(p,k) where p is the smallest integer with
   triangular(p) ≥ n, k = n – triangular(p-1),
   and T(p,k) = k^p – k! * S(p,k) (S = Stirling numbers of the second kind). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pow_seq_idx.h"
#include "../utils/triangular.h"

/* Forward declarations for helper routines */
static idx_t triangular(idx_t p);
static idx_t find_row(idx_t n);
static seq_t pow_seq_idx(seq_t base, idx_t exp);

/* Global tables for Stirling numbers and factorials */
static seq_t **S_global = NULL;    /* S_global[i][k] = S(i,k) */
static seq_t *fact_global = NULL;  /* fact_global[i] = i!      */
static idx_t computed_max_global = 0;  /* Largest n computed */

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* Determine the row index p given a global enumeration index n. */
static idx_t find_row(idx_t n)
{
    idx_t p = 1;
    while (triangular(p) < n)
        ++p;
    return p;
}

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* Expand Stirling table and factorial array up to nmax (inclusive). */
static void compute_stirling_global(idx_t nmax)
{
    if (nmax <= computed_max_global)
        return;

    /* Reallocate arrays with size nmax+1 */
#ifdef __cplusplus
    S_global    = reinterpret_cast<seq_t**>(realloc(S_global,
                           sizeof(seq_t*) * (nmax + 1)));
    fact_global = reinterpret_cast<seq_t*>(realloc(fact_global,
                           sizeof(seq_t) * (nmax + 1)));
#else
    S_global    = (seq_t**)realloc(S_global,
                           sizeof(seq_t*) * (nmax + 1));
    fact_global = (seq_t*)realloc(fact_global,
                           sizeof(seq_t) * (nmax + 1));
#endif
    if (computed_max_global == 0) {

        /* Base case S(0,0)=1, 0! = 1 */
#ifdef __cplusplus
        S_global[0] = new seq_t[1];
#else
        S_global[0] = (seq_t*)calloc(1, sizeof(seq_t));
#endif
        S_global[0][0] = (seq_t)1;
        fact_global[0] = (seq_t)1;
    }

    /* Build Stirling numbers and factorials for i = computed_max_global+1 … nmax */
    for (idx_t i = computed_max_global + 1; i <= nmax; ++i) {
#ifdef __cplusplus
        S_global[i] = new seq_t[i + 1];
#else
        S_global[i] = (seq_t*)calloc(i + 1, sizeof(seq_t));
#endif
        fact_global[i] = fact_global[i - 1] * (seq_t)i;

        S_global[i][0] = (seq_t)0;
        for (idx_t k = 1; k < i; ++k) {
            S_global[i][k] = S_global[i - 1][k - 1] +
                             (seq_t)k * S_global[i - 1][k];
        }
        S_global[i][i] = (seq_t)1;
    }
    computed_max_global = nmax;
}

/* ------------------------------------------------------------------ */
/* OEIS sequence A101030: triangle read by rows. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* offset 1 – index 0 unused */

    /* Determine (p,k) from global index n */
    idx_t p = find_row(n);
    idx_t k = n - triangular(p - 1);

    /* Ensure tables cover needed indices */
    compute_stirling_global((idx_t)((p > k) ? p : k));

    /* Compute T(p,k) = k^p – k! * S(p,k) */
    seq_t power   = pow_seq_idx((seq_t)k, p);
    seq_t term    = fact_global[k] * S_global[p][k];
    seq_t result  = power - term;

    return result;
}
